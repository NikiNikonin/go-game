import { useEffect, useState } from "react";
import { Button } from "@headlessui/react";
import SelectionColumn from "../components/SelectionColumn";
import GoBoard from "../components/GoBoard";

const botLevels = ["Easy", "Medium", "Hard"];
const boardSizes = ["9x9", "13x13", "19x19"];
const API_URL = "http://localhost:8080";

export default function Solo() {
    const [selectedBotLevel, setSelectedBotLevel] = useState(0);
    const [selectedBoardSize, setSelectedBoardSize] = useState(0);
    const [isPlaying, setPlaying] = useState(false);

    useEffect(() => {
        function handleOnBeforeUnload(event: BeforeUnloadEvent) {
            if (!isPlaying) return;
            event.preventDefault();
            return (event.returnValue = '');
        }

        window.addEventListener('beforeunload', handleOnBeforeUnload, { capture: true })
        return () => {
            window.removeEventListener('beforeunload', handleOnBeforeUnload, { capture: false })
        }
    }, [isPlaying])

    async function handlePlayButtonClick() {
        setPlaying(true);
        try {
            const response = await fetch(`${API_URL}/new-game`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify({ size: selectedBoardSize, _1st_bot_difficulty: selectedBotLevel, _2st_bot_difficulty: -1 }),
            });

            const data = await response.json();
            console.log("New Game:", data);
        } catch (error) {
            console.error("Error starting a new game:", error);
        }
    }

    return (
        <div>
            <GoBoard size={selectedBoardSize} playing={isPlaying} players_cnt={1} />
            <div className={`z-10 fixed top-0 left-0 bg-neutral-900/60 h-2/3 w-full transition-opacity duration-300 ${isPlaying ? 'opacity-0' : 'opacity-100'}`} />
            <div className={`z-10 fixed bottom-0 left-0 right-0 bg-emerald-900/90 h-1/3 transition-transform duration-300 ${isPlaying ? 'translate-y-full' : 'translate-y-0'}`}>
                <div className="flex flex-col items-center justify-between min-h-screen text-white">
                    <div className="flex space-x-10 pt-15">
                        {/* bot difficulty */}
                        <SelectionColumn
                            title="bot"
                            options={botLevels}
                            selectedIndex={selectedBotLevel}
                            onSelect={setSelectedBotLevel}
                        />

                        {/* Board size */}
                        <SelectionColumn
                            title="Board size"
                            options={boardSizes}
                            selectedIndex={selectedBoardSize}
                            onSelect={setSelectedBoardSize}
                        />

                        {/* Play button */}
                        <div className="flex items-center">
                            <Button
                                onClick={() => handlePlayButtonClick()}
                                style={{ width: '160px', borderRadius: '25px' }}
                                className="relative rounded-full bg-blue-500 text-white text-lg font-semibold shadow-lg hover:bg-blue-600 duration-300"
                            >
                                Play
                            </Button>
                        </div>
                    </div>
                </div>
                {isPlaying}
            </div>
        </div>
    );

};
