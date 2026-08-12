import { useState } from 'react'
import { analizarComando } from '../utils/analizador'
import '../styles/Analizador.css'

function Analizador() {
  const [entrada, setEntrada] = useState('')
  const [resultado, setResultado] = useState(null)
  const [resultados, setResultados] = useState(null)

  const manejarAnalisis = (e) => {
    e.preventDefault()
    if (entrada.trim()) {
      const res = analizarComando(entrada)
      setResultado(res)
      setResultados(null)
    }
  }

  const manejarArchivo = async (e) => {
    const archivo = e.target.files[0]
    if (archivo) {
      const texto = await archivo.text()
      const comandos = texto.split('\n').filter(l => l.trim() && !l.trim().startsWith('#'))

      const resultadosAnalisis = comandos.map((cmd, idx) => ({
        numero: idx + 1,
        comando: cmd.trim(),
        resultado: analizarComando(cmd.trim())
      }))

      setResultados(resultadosAnalisis)
      setResultado(null)
    }
  }

  const limpiar = () => {
    setEntrada('')
    setResultado(null)
    setResultados(null)
  }

  return (
    <div className="analizador">
      <div className="modo-selector">
        <div className="modo">
          <h4>Comando Individual</h4>
          <form onSubmit={manejarAnalisis}>
            <input
              type="text"
              value={entrada}
              onChange={(e) => setEntrada(e.target.value)}
              placeholder="Ingresa un comando EXT2 (ej: mkdisk -size=100 -path=/disco.mia)"
              className="entrada"
            />
            <div className="botones">
              <button type="submit">Analizar</button>
              <button type="button" onClick={limpiar} className="secundario">Limpiar</button>
            </div>
          </form>
        </div>

        <div className="modo">
          <h4>Cargar Archivo (.txt)</h4>
          <input
            type="file"
            accept=".txt"
            onChange={manejarArchivo}
            className="entrada-archivo"
          />
          {resultados && (
            <button type="button" onClick={limpiar} className="secundario">Limpiar</button>
          )}
        </div>
      </div>

      {resultado && (
        <div className={`resultado ${resultado.lexico.exito ? 'exitoso' : 'error'}`}>
          <h3>Análisis Léxico</h3>
          {resultado.lexico.exito ? (
            <div className="tokens">
              {resultado.lexico.tokens.map((t, i) => (
                <span key={i} className="token">{t.tipo}:{t.valor}</span>
              ))}
            </div>
          ) : (
            <p className="mensaje-error">{resultado.lexico.error}</p>
          )}

          {resultado.lexico.exito && (
            <>
              <h3>Análisis Sintáctico</h3>
              {resultado.sintactico.valido ? (
                <p className="valido">✓ Comando válido</p>
              ) : (
                <ul className="errores">
                  {resultado.sintactico.errores.map((e, i) => (
                    <li key={i}>✗ {e}</li>
                  ))}
                </ul>
              )}
            </>
          )}
        </div>
      )}

      {resultados && (
        <div className="resultados-multiples">
          <h3>Resultados del Archivo ({resultados.length} comandos)</h3>
          <div className="tabla-resultados">
            {resultados.map((item) => (
              <div key={item.numero} className={`fila-resultado ${item.resultado.sintactico.valido ? 'exitoso' : 'error'}`}>
                <div className="numero-cmd">#{item.numero}</div>
                <div className="cmd-texto">{item.comando}</div>
                <div className="estado">
                  {item.resultado.sintactico.valido ? (
                    <span className="valido-badge">✓ Válido</span>
                  ) : (
                    <span className="error-badge">✗ Error</span>
                  )}
                </div>
                {!item.resultado.sintactico.valido && (
                  <div className="errores-inline">
                    {item.resultado.sintactico.errores.map((e, i) => (
                      <span key={i} className="error-msg">{e}</span>
                    ))}
                  </div>
                )}
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  )
}

export default Analizador
