import React, { useEffect, useState, useRef, useMemo, useCallback } from 'react';
import { motion } from 'framer-motion';
import BlackGoStone from '../assets/black-go-stone.svg'
import WhiteGoStone from '../assets/white-go-stone.svg'
import NeutralGoStone from '../assets/neutral-go-stone.svg'

type StoneColor = "White" | "Black" | "Neutral";

const GoBoard = ({ size = 0, playing = false, players_cnt }: { size: number, playing: boolean, players_cnt: number }) => {
    const API_URL = "http://localhost:8080";
    const [prevSize, setPrevSize] = useState(size);
    const stoneScale = 0.7;
    const bricksCnt = [8, 12, 18];
    const absoluteSide = 768;
    const bricksLayoutSide = absoluteSide - 2;
    const cellSide = bricksLayoutSide / bricksCnt[size];
    const stoneRadius = cellSide * stoneScale / 2;
    const gradientRadius = cellSide / 1.5;
    const brickSide = cellSide - 2;
    const [gradient, setGradient] = useState<string>();
    const rectRef = useRef<HTMLDivElement | null>(null);
    const bricks = useMemo(() => Array.from({ length: bricksCnt[Math.max(size, prevSize)] ** 2 }), [size, prevSize]);
    const hatNumbers = [[1, 2, 3, 4, 5, 6, 7, 8, 9], [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13], [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]];
    const sideBarChars = [['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J'], ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N'], ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T']];
    const [board, setBoard] = useState<StoneColor[][]>(
        Array.from({ length: bricksCnt[size] + 1 }, () => Array(bricksCnt[size] + 1).fill("Neutral"))
    );
    const [currentPlayer, setCurrentPlayer] = useState<("Black" | "White")>("Black");
    const stonesBackgrounds = {
        Black: BlackGoStone,
        White: WhiteGoStone,
        Neutral: NeutralGoStone,
    };
    const lastMousePosition = useRef({ x: 0, y: 0 });
    const whiteTextColor = "#bfb082";
    const blackTextColor = "#cc6acc";

    const getTextMetrics = (text: string, fontSize: string = '13px', fontFamily: string = 'Arial') => {
        const canvas = document.createElement('canvas');
        const context = canvas.getContext('2d');
        if (!context)
            throw new Error('Anable to get canvas 2d context.');
        context.font = `${fontSize} ${fontFamily}`;
        const metrics = context.measureText(text);
        return metrics;
    }

    async function makePlayerMove(move: string) {
        if (players_cnt === 0 || players_cnt === 1 && currentPlayer === "White") return;
        console.log(move);

        try {
            const response = await fetch(`${API_URL}/player-move`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify({ move }),
            });

            const data = await response.json();
            if (data === false) {
                console.log("Invalid move!");
            } else {
                setBoard(data.board);
                setCurrentPlayer((prev) => (prev === "Black" ? "White" : "Black"));
            }
        } catch (error) {
            console.error("Error making a move:", error);
        }
    }

    function getMoveString(row: number, col: number): string {
        return `${sideBarChars[size][row]}${hatNumbers[size][col]}`;
    }

    // function getMoveFromString(move: string) : {row: number, col: number} {
    //     const match = move.match(/^([A-Za-z]+)(\d+)$/);

    //     let row = 0, col = 0;
    //     if (!match) return {row, col};
    //     const letterPart = match[1].toUpperCase();
    //     const numberPart = parseInt(match[2], 10);

    //     const charCode = letterPart.charCodeAt(0) - 'A'.charCodeAt(0);

    //     if (charCode >= 8) {
    //         row += charCode - 1;
    //     } else {
    //         row += charCode;
    //     }

    //     col = numberPart - 1;

    //     return { row, col };
    // }

    const handleClick = useCallback((row: number, col: number) => {
        const move = getMoveString(row, col);
        makePlayerMove(move);
    }, []);

    useEffect(() => {
        let animationFrame: number;

        if (prevSize !== size) {
            setTimeout(() => {
                setBoard(Array.from({ length: bricksCnt[size] + 1 }, () => Array(bricksCnt[size] + 1).fill("Neutral")));
                setPrevSize(size);
            }, 300)
        }

        const updateGradient = () => {
            if (!rectRef.current || !lastMousePosition.current) return;

            const rect = rectRef.current.getBoundingClientRect();
            const offsetX = lastMousePosition.current.x - rect.left;
            const offsetY = lastMousePosition.current.y - rect.top;

            setGradient(
                `radial-gradient(circle at ${offsetX}px ${offsetY}px, White, rgba(50, 50, 50, 1) ${gradientRadius}px)`
            );
            animationFrame = requestAnimationFrame(updateGradient);
        };

        const handleMouseMove = (e: MouseEvent) => {
            lastMousePosition.current = { x: e.clientX, y: e.clientY };
        };

        window.addEventListener("mousemove", handleMouseMove);
        animationFrame = requestAnimationFrame(updateGradient);


        return () => {
            window.removeEventListener("mousemove", handleMouseMove);
            cancelAnimationFrame(animationFrame);
        };
    }, [size]);

    useEffect(() => {
        if (size === 0) {
            board[1][1] = "White";
            board[2][2] = "White";
            board[1][2] = "Black";
            board[2][1] = "Black";

            board[6][6] = "Black";
            board[7][7] = "Black";
            board[6][7] = "White";
            board[7][6] = "White";
            return;
        }
        if (size === 1) {
            board[2][2] = "White";
            board[3][3] = "White";
            board[2][3] = "Black";
            board[3][2] = "Black";

            board[2][9] = "White";
            board[3][10] = "White";
            board[2][10] = "Black";
            board[3][9] = "Black";

            board[9][2] = "White";
            board[10][3] = "White";
            board[10][2] = "Black";
            board[9][3] = "Black";

            board[9][9] = "White";
            board[10][10] = "White";
            board[9][10] = "Black";
            board[10][9] = "Black";
            return;
        }
        if (size === 2) {

            return;
        }
    }, [playing]);

    useEffect(() => {
        if (!playing) return;
        if (players_cnt === 2 || (players_cnt === 1 && currentPlayer === "Black")) return;
    
        const fetchBotMove = async () => {
            try {
                const response = await fetch(`${API_URL}/bot-move`, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({ player: currentPlayer })
                });
    
                if (!response.ok) {
                    const errorText = await response.text();
                    throw new Error(`Server error: ${errorText}`);
                }
    
                const data = await response.json();
                setBoard(data.board);
                setCurrentPlayer(prev => prev === "Black" ? "White" : "Black");
            } catch (error) {
                console.error("Bot move error:", error);
            }
        };
    
        fetchBotMove();
    }, [currentPlayer, playing]);

    return (
        <div className="relative w-screen h-screen overflow-hidden">

            {/* top/right wall */}
            <div className="absolute top-0 left-0" style={{ width: `calc(50% - ${absoluteSide / 2}px)`, height: `calc(50% - ${absoluteSide / 2}px)`, background: 'rgb(40, 40, 40)', zIndex: 2 }} />
            <div className="flex absolute top-0" style={{ left: `calc(50% - ${absoluteSide / 2}px)`, width: `${absoluteSide}px`, height: `calc(50% - ${absoluteSide / 2}px)`, background: 'rgb(40, 40, 40)', zIndex: 3 }}>
                {hatNumbers[size].map((number) => (
                    <motion.span
                        key={number}
                        initial={{ opacity: 0 }}
                        animate={{ opacity: playing ? 1 : 0, color: `${currentPlayer === "Black" ? blackTextColor : whiteTextColor}` }}
                        transition={{ duration: 0.3, ease: "linear" }}
                        style={{
                            position: 'absolute',
                            bottom: `${stoneRadius + 9}px`,
                            fontSize: '13px',
                            left: `${(number - 1) * (brickSide + 2) - getTextMetrics(number.toString()).width / 2}px`,
                        }}
                    >
                        {number}
                    </motion.span>
                ))}
            </div>
            <div className="absolute top-0 h-full" style={{ left: `calc(50% + ${absoluteSide / 2}px)`, width: `calc(50% - ${absoluteSide / 2}px)`, background: 'rgb(40, 40, 40)', zIndex: 2 }} />

            {/* left/bottom wall */}
            <div className="absolute top-0 left-0" style={{ height: `calc(50% - ${absoluteSide / 2}px)`, width: `calc(50% - ${absoluteSide / 2}px)`, background: 'rgba(40, 40, 40, 0)', zIndex: 2 }} />
            <div className="absolute left-0 h-full" style={{ top: `calc(50% - ${absoluteSide / 2}px)`, width: `calc(50% - ${absoluteSide / 2}px)`, background: 'rgb(40, 40, 40)', zIndex: 3 }}>
                {sideBarChars[size].map((char, index) => (
                    <motion.span
                        key={char}
                        initial={{ opacity: 0 }}
                        animate={{ opacity: playing ? 1 : 0, color: `${currentPlayer === "Black" ? blackTextColor : whiteTextColor}` }}
                        transition={{ duration: 0.3, ease: "linear" }}
                        style={{
                            position: 'absolute',
                            marginTop: '-9px',
                            fontSize: '13px',
                            top: `${index * (brickSide + 2)}px`,
                            right: `${stoneRadius + 13}px`,
                        }}
                    >
                        {char}
                    </motion.span>
                ))}
            </div>
            <div className="absolute bottom-0 left-0 w-full" style={{ height: `calc(50% - ${absoluteSide / 2}px)`, background: 'rgb(40, 40, 40)', zIndex: 2 }} />

            <div
                ref={rectRef}
                style={{
                    top: `calc(50% - ${absoluteSide / 2}px)`,
                    left: `calc(50% - ${absoluteSide / 2}px)`,
                    width: `${absoluteSide}px`,
                    height: `${absoluteSide}px`,
                    background: gradient,
                    position: 'relative',
                    display: 'relative',
                    alignItems: 'center',
                    justifyContent: 'center',
                }}
            >
                <motion.div
                    key={size}
                    initial={{ scale: 1.5 ** (size - prevSize) }}
                    animate={{ scale: 1 }}
                    transition={{ duration: 0.3, ease: "easeInOut" }}
                    className="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 grid"
                    style={{
                        transformOrigin: "center",
                        willChange: "transform",
                        display: "grid",
                        gridTemplateColumns: `repeat(${bricksCnt[Math.max(prevSize, size)]}, ${brickSide}px)`,
                        gridTemplateRows: `repeat(${bricksCnt[Math.max(prevSize, size)]}, ${brickSide}px)`,
                        gap: "2px"
                    }}
                >
                    {bricks.map((_, index) => (
                        <div
                            key={index}
                            style={{
                                width: `${brickSide}px`,
                                height: `${brickSide}px`,
                                backgroundColor: 'Black',
                            }}
                        />
                    ))}
                </motion.div>

                {playing && (
                    board.map((row, rowIndex) =>
                        row.map((cell, colIndex) =>
                            <GoStone key={`${rowIndex}-${colIndex}`} row={rowIndex} col={colIndex} color={cell} stoneRadius={stoneRadius} cellSide={cellSide} onClick={handleClick} stonesBackgrounds={stonesBackgrounds} />
                        )
                    ))}
            </div>
        </div>
    );
};

export default GoBoard;

const GoStone = React.memo(
    ({
        row,
        col,
        color,
        stoneRadius,
        cellSide,
        onClick,
        stonesBackgrounds,
    }: {
        row: number;
        col: number;
        color: StoneColor;
        stoneRadius: number;
        cellSide: number;
        onClick: (row: number, col: number) => void;
        stonesBackgrounds: { Black: string; White: string; Neutral: string };
    }) => (
        <div
            className="absolute rounded-full cursor-pointer"
            onClick={() => onClick(row, col)}
            style={{
                width: `${stoneRadius * 2}px`,
                height: `${stoneRadius * 2}px`,
                top: `${row * cellSide - stoneRadius + 1}px`,
                left: `${col * cellSide - stoneRadius + 1}px`,
                zIndex: 11,
            }}
        >
            <img src={stonesBackgrounds[color]} alt={color} loading="lazy" />
        </div>
    )
);