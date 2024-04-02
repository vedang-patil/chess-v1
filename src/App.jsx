import { useEffect, useState } from "react";
import Piece from "./components/Piece";

function App() {
    const [fenString, setFenString] = useState("");
    useEffect(() => {
        setFenString(board.getAsFenString());
    }, []);

    const parts = fenString.split(" ");
    const pieces = Array(64).fill(" ");
    let idx = 0;

    for (const c of parts[0]) {
        if (c == "/") continue;

        if (!isNaN(c)) {
            idx += parseInt(c);
        } else {
            pieces[idx] = c;
            idx++;
        }
    }

    return (
        <div>
            <div className="board">
                {pieces.map((piece, index) => (
                    <div
                        key={index}
                        className={
                            Math.floor(index / 8) % 2 == (index % 8) % 2
                                ? "whiteSquare"
                                : "blackSquare"
                        }
                    >
                        <Piece letter={piece} />
                    </div>
                ))}
            </div>
        </div>
    );
}

export default App;
