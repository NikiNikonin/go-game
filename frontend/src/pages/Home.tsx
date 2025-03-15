import { Link } from "react-router-dom";
import { Button } from "@headlessui/react";
import goIcon from '../assets/go-icon.png';

export default function Home() {
    return (
        <div className="flex flex-col items-center justify-center min-h-screen bg-transparent">
            <img src={goIcon} alt="Go Icon" className="w-80 h-80 mb-6" />
            <div className="flex space-x-5">
                <Link to="/auto">
                    <Button
                        className="relative overflow-hidden px-6 py-3 rounded-full bg-blue-500 text-white text-lg font-semibold shadow-lg hover:bg-blue-600 transition-all duration-300"
                    >
                        Auto
                        <span className="absolute inset-0 bg-blue-700 opacity-0 hover:opacity-20 transition-all"></span>
                    </Button>
                </Link>
                <Link to="/solo">
                    <Button
                        className="relative overflow-hidden px-6 py-3 rounded-full bg-blue-500 text-white text-lg font-semibold shadow-lg hover:bg-blue-600 transition-all duration-300"
                    >
                        Solo
                        <span className="absolute inset-0 bg-blue-700 opacity-0 hover:opacity-20 transition-all"></span>
                    </Button>
                </Link>
                <Link to="/duo">
                    <Button
                        className="relative overflow-hidden px-6 py-3 rounded-full bg-blue-500 text-white text-lg font-semibold shadow-lg hover:bg-blue-600 transition-all duration-300"
                    >
                        Duo
                        <span className="absolute inset-0 bg-blue-700 opacity-0 hover:opacity-20 transition-all"></span>
                    </Button>
                </Link>
            </div>
        </div>
    );
}
