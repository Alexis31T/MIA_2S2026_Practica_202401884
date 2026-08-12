const COMANDOS = ['MKDISK', 'RMDISK', 'FDISK', 'MOUNT', 'MKFS', 'MKUSR', 'RMUSR', 'MKFILE']
const PARAMETROS = ['-size', '-fit', '-unit', '-path', '-type', '-name', '-id', '-user', '-pass', '-grp', '-r', '-cont']

function analizarLexico(entrada) {
  const palabras = entrada.trim().split(/\s+/)
  if (!palabras.length) return { exito: false, tokens: [], error: 'Entrada vacía' }

  const cmd = palabras[0].toUpperCase()
  if (!COMANDOS.includes(cmd)) {
    return { exito: false, tokens: [], error: `Comando inválido: ${palabras[0]}` }
  }

  const tokens = [{ tipo: 'COMANDO', valor: cmd }]

  for (let i = 1; i < palabras.length; i++) {
    const p = palabras[i]
    if (p.startsWith('-')) {
      const [param, ...valor] = p.split('=')
      const paramLower = param.toLowerCase()

      if (!PARAMETROS.includes(paramLower)) {
        return { exito: false, tokens: [], error: `Parámetro inválido: ${paramLower}` }
      }

      tokens.push({ tipo: 'PARAMETRO', valor: paramLower })
      if (valor.length) {
        tokens.push({ tipo: 'VALOR', valor: valor.join('=') })
      }
    } else {
      tokens.push({ tipo: 'VALOR', valor: p })
    }
  }

  return { exito: true, tokens }
}

function validarMKDISK(params) {
  const errores = []
  if (!params['-size']) errores.push('Falta -size')
  else if (isNaN(parseInt(params['-size'])) || parseInt(params['-size']) <= 0) errores.push('-size debe ser positivo')
  if (!params['-path']) errores.push('Falta -path')
  if (params['-unit'] && !['K', 'M'].includes(params['-unit'])) errores.push('-unit debe ser K o M')
  if (params['-fit'] && !['BF', 'FF', 'WF'].includes(params['-fit'])) errores.push('-fit debe ser BF, FF o WF')
  return { valido: errores.length === 0, errores }
}

function validarRMDISK(params) {
  const errores = []
  if (!params['-path']) errores.push('Falta -path')
  return { valido: errores.length === 0, errores }
}

function validarFDISK(params) {
  const errores = []
  if (!params['-size']) errores.push('Falta -size')
  else if (isNaN(parseInt(params['-size'])) || parseInt(params['-size']) <= 0) errores.push('-size debe ser positivo')
  if (!params['-path']) errores.push('Falta -path')
  if (!params['-name']) errores.push('Falta -name')
  if (params['-unit'] && !['B', 'K', 'M'].includes(params['-unit'])) errores.push('-unit debe ser B, K o M')
  if (params['-type'] && !['P', 'E', 'L'].includes(params['-type'])) errores.push('-type debe ser P, E o L')
  if (params['-fit'] && !['BF', 'FF', 'WF'].includes(params['-fit'])) errores.push('-fit debe ser BF, FF o WF')
  return { valido: errores.length === 0, errores }
}

function validarMOUNT(params) {
  const errores = []
  if (!params['-path']) errores.push('Falta -path')
  if (!params['-name']) errores.push('Falta -name')
  return { valido: errores.length === 0, errores }
}

function validarMKFS(params) {
  const errores = []
  if (!params['-id']) errores.push('Falta -id')
  if (params['-type'] && params['-type'] !== 'full') errores.push('-type debe ser full')
  return { valido: errores.length === 0, errores }
}

function validarMKUSR(params) {
  const errores = []
  if (!params['-user']) errores.push('Falta -user')
  else if (params['-user'].length > 10) errores.push('-user máximo 10 caracteres')
  if (!params['-pass']) errores.push('Falta -pass')
  else if (params['-pass'].length > 10) errores.push('-pass máximo 10 caracteres')
  if (!params['-grp']) errores.push('Falta -grp')
  else if (params['-grp'].length > 10) errores.push('-grp máximo 10 caracteres')
  return { valido: errores.length === 0, errores }
}

function validarRMUSR(params) {
  const errores = []
  if (!params['-user']) errores.push('Falta -user')
  return { valido: errores.length === 0, errores }
}

function validarMKFILE(params) {
  const errores = []
  if (!params['-path']) errores.push('Falta -path')
  if (params['-size'] && (isNaN(parseInt(params['-size'])) || parseInt(params['-size']) < 0)) errores.push('-size no puede ser negativo')
  return { valido: errores.length === 0, errores }
}

function analizarSintactico(tokens) {
  const cmd = tokens[0].valor
  const params = {}

  for (let i = 1; i < tokens.length; i++) {
    if (tokens[i].tipo === 'PARAMETRO') {
      params[tokens[i].valor] = tokens[i + 1]?.tipo === 'VALOR' ? tokens[i + 1].valor : ''
      if (tokens[i + 1]?.tipo === 'VALOR') i++
    }
  }

  switch (cmd) {
    case 'MKDISK': return validarMKDISK(params)
    case 'RMDISK': return validarRMDISK(params)
    case 'FDISK': return validarFDISK(params)
    case 'MOUNT': return validarMOUNT(params)
    case 'MKFS': return validarMKFS(params)
    case 'MKUSR': return validarMKUSR(params)
    case 'RMUSR': return validarRMUSR(params)
    case 'MKFILE': return validarMKFILE(params)
    default: return { valido: false, errores: ['Comando desconocido'] }
  }
}

export function analizarComando(entrada) {
  const lexico = analizarLexico(entrada)
  let sintactico = { valido: false, errores: [] }

  if (lexico.exito) {
    sintactico = analizarSintactico(lexico.tokens)
  }

  return { lexico, sintactico }
}
