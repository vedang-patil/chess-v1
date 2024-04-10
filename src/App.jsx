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

    const attemptMove = (event) => {
        if (!event.over) return;
        const from = parseInt(event.active.id.substring(6));
        const to = parseInt(event.over.id.substring(7));
        board.move(new Module.Pair(from, to));
        setReloardBoard(!reloadBoard);
    };

    return (
        <DndContext onDragEnd={attemptMove}>
            <div className="board">
                {pieces.map((piece, index) => (
                    <Square
                        key={index}
                        index={(7 - Math.floor(index / 8)) * 8 + (index % 8)}
                        pieceLetter={piece}
                    />
                ))}
            </div>
        </DndContext>
    );
}

export default App;
