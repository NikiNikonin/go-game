import './App.css'
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Home from "./pages/Home";
import Auto from "./pages/Auto";
import Solo from "./pages/Solo";
import Duo from "./pages/Duo";

export default function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/auto" element={<Auto />} />
        <Route path="/solo" element={<Solo />} />
        <Route path="/duo" element={<Duo />} />
      </Routes>
    </Router>
  );
}