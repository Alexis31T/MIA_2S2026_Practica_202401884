import { useState } from 'react'
import { analizarComando } from '../utils/analizador'
import '../styles/Analizador.css'

function Analizador() {
  const [entrada, setEntrada] = useState('')
  const [resultado, setResultado] = useState(null)

  const manejarAnalisis = (e) => {
    e.preventDefault()
    if (entrada.trim()) {
      const res = analizarComando(entrada)
      setResultado(res)
    }
  }

  const limpiar = () => {
    setEntrada('')
    setResultado(null)
  }

  return (
    <div className="analizador">
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
    </div>
  )
}

export default Analizador
