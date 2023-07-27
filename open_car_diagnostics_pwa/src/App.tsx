import React from 'react'
import { Outlet } from 'react-router-dom'
import ReloadPrompt from './ReloadPrompt'
import './App.css'

function App() {
  // replaced dyanmicaly
  const date = '__DATE__'

  return (
    <main className="App">
      <img src="/favicon.svg" alt="Open Car Diagnostics Logo" width="100" height="100" />
      <h1 className="Home-title">Open Car Diagnostics!</h1>
      <div className="Home-built">Built at: {date}</div>
      <Outlet />
      <ReloadPrompt />
    </main>
  )
}

export default App
