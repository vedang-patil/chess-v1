import { useEffect, useState } from "react";
import Square from "./components/Square";
import { DndContext } from "@dnd-kit/core";

function App() {
    const [reloadBoard, setReloardBoard] = useState(false);
    const [fenString, setFenString] = useState("");
    const [isPlayerWhite, setIsPlayerWhite] = useState(true);

    useEffect(() => {
        setFenString(board.getAsFenString());
    }, [reloadBoard]);

    const parts = fenString.split(" ");
    const pieces = Array(64).fill();
    let idx = 0;

    for (const c of parts[0]) {
        if (c == "/") continue;

        if (!isNaN(c)) {
            idx += parseInt(c);
        } else {
            if (c !== " ") pieces[idx] = c;
            idx++;
        }
    }

    if (!isPlayerWhite) pieces.reverse();
    const isPlayerTurn = (parts[1] == "w") === isPlayerWhite;

    const move = (from, to) => {
        board.move(new Module.Pair(from, to));
        setReloardBoard(!reloadBoard);
    };

    const attemptMove = (event) => {
        if (!event.over) return;
        const from = parseInt(event.active.id.substring(6));
        const to = parseInt(event.over.id.substring(7));
        move(from, to);
    };

    const playBestMove = () => {
        let bestMove = null;
        let bestScore = parts[1] === "w" ? -Infinity : Infinity;
        const moveChoices = board.getMoves();
        for (let i = 0; i < moveChoices.size(); i++) {
            const moveBoard = Module.makeCopy(board);
            moveBoard.move(moveChoices.get(i));
            let score = Module.minimax(moveBoard, 0, parts[1] === "w");
            if (parts[1] === "w" && score > bestScore) {
                bestMove = moveChoices.get(i);
                bestScore = score;
            } else if (parts[1] === "b" && score < bestScore) {
                bestMove = moveChoices.get(i);
                bestScore = score;
            }
        }

        console.log(bestMove);

        if (bestMove) {
            move(bestMove.first, bestMove.second);
        }
    };

    return (
        <DndContext onDragEnd={attemptMove}>
            <div className="board">
                {pieces.map((piece, index) => (
                    <Square
                        key={index}
                        index={
                            isPlayerWhite
                                ? (7 - Math.floor(index / 8)) * 8 + (index % 8)
                                : 63 -
                                  ((7 - Math.floor(index / 8)) * 8 +
                                      (index % 8))
                        }
                        pieceLetter={piece}
                    />
                ))}
            </div>
            <button onClick={() => setIsPlayerWhite(!isPlayerWhite)}>
                Switch
            </button>
            <button onClick={() => setReloardBoard(!reloadBoard)}>
                Reload
            </button>
            <button onClick={playBestMove}>Play Best Move</button>
        </DndContext>
    );
}

export default App;
