import { useEffect, useState, useRef, useCallback, useMemo } from 'react';
import { motion } from 'framer-motion';
import BlackGoStone from '../assets/black-go-stone.svg';
import WhiteGoStone from '../assets/white-go-stone.svg';
import TransparentGoStone from '../assets/transparent-go-stone.svg';

const stoneScale = 0.7;
const bricksCnt = [8, 12, 18]; // Количество клеток на доске

const stonesBackgrounds = {
    black: BlackGoStone,
    white: WhiteGoStone,
    transparent: TransparentGoStone,
};

const GoBoard = ({ size = 0, playing = false }: { size: number; playing: boolean }) => {
    const bricksSize = bricksCnt[size]; // Текущий размер доски
    const absoluteSide = 768;
    const bricksLayoutSide = absoluteSide - 2;
    const cellSide = bricksLayoutSide / bricksSize;
    const stoneRadius = cellSide * stoneScale / 2;
    const brickSide = cellSide - 2;
    const rectRef = useRef<HTMLDivElement | null>(null);
    const [board, setBoard] = useState<("transparent" | "black" | "white")[][]>(
        () => Array.from({ length: bricksSize }, () => Array(bricksSize).fill("transparent"))
    );
    const [currentPlayer, setCurrentPlayer] = useState<"black" | "white">("black");
    const [gradient, setGradient] = useState<string>();

    // Кэшируем список клеток, чтобы избежать их пересоздания при каждом ререндере
    const bricks = useMemo(() => Array.from({ length: bricksSize ** 2 }), [bricksSize]);

    // Хранение мыши для плавного обновления градиента
    const lastMousePosition = useRef({ x: 0, y: 0 });



    useEffect(() => {
        let animationFrame: number;

        const updateGradient = () => {
            setGradient(
                `radial-gradient(circle at ${lastMousePosition.current.x}px ${lastMousePosition.current.y}px, white, rgba(50, 50, 50, 1) ${cellSide}px)`
            );
            animationFrame = requestAnimationFrame(updateGradient);
        };

        const handleMouseMove = (e: MouseEvent) => {
            lastMousePosition.current = { x: e.clientX, y: e.clientY };
        };

        if (playing) {
            window.addEventListener('mousemove', handleMouseMove);
            animationFrame = requestAnimationFrame(updateGradient);
        }

        return () => {
            window.removeEventListener('mousemove', handleMouseMove);
            cancelAnimationFrame(animationFrame);
        };
    }, [playing, cellSide]);

    return (
        <div className="relative w-screen h-screen overflow-hidden">
            {/* Игровая доска */}
            <div
                ref={rectRef}
                className="absolute"
                style={{
                    top: `calc(50% - ${absoluteSide / 2}px)`,
                    left: `calc(50% - ${absoluteSide / 2}px)`,
                    width: `${absoluteSide}px`,
                    height: `${absoluteSide}px`,
                    background: gradient,
                    display: "grid",
                    gridTemplateColumns: `repeat(${bricksSize}, ${brickSide}px)`,
                    gridTemplateRows: `repeat(${bricksSize}, ${brickSide}px)`,
                    gap: "2px",
                }}
            >
                {bricks.map((_, index) => (
                    <div key={index} className="bg-black" />
                ))}
            </div>

            {/* Камни */}
            {playing &&
                board.map((row, rowIndex) =>
                    row.map(
                        (cell, colIndex) =>
                            cell !== "transparent" && (
                                <GoStone
                                    key={`${rowIndex}-${colIndex}`}
                                    row={rowIndex}
                                    col={colIndex}
                                    color={cell}
                                    stoneRadius={stoneRadius}
                                    cellSide={cellSide}
                                    onClick={handleClick}
                                />
                            )
                    )
                )}
        </div>
    );
};

// Мемозированный компонент GoStone (избегаем ненужных ререндеров)
const GoStone = React.memo(
    ({
        row,
        col,
        color,
        stoneRadius,
        cellSide,
        onClick,
    }: {
        row: number;
        col: number;
        color: "black" | "white";
        stoneRadius: number;
        cellSide: number;
        onClick: (row: number, col: number) => void;
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

export default GoBoard;
