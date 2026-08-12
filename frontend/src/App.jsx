import { useState } from 'react'
import Analizador from './components/Analizador'
import './styles/App.css'

function App() {
  return (
    <div className="app">
      <div className="contenedor">
        <h1>Analizador Léxico y Sintáctico EXT2</h1>
        <p className="subtitulo">Valida comandos de administración de discos</p>
        <Analizador />
      </div>
    </div>
  )
}

export default App
