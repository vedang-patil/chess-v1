import { useEffect, useState } from "react";

function Piece({ letter }) {
    const pieceColourClass =
        letter == " "
            ? ""
            : letter.toUpperCase() === letter
            ? "piece-white"
            : "piece-black";
    return (
        <div className={"piece " + pieceColourClass}>
            {letter == " " ? (
                <></>
            ) : (
                <img src={"/pieces/" + letter.toUpperCase() + ".webp"}></img>
            )}
        </div>
    );
}

export default Piece;
