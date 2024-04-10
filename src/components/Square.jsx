import { useEffect, useState } from "react";
import { useDroppable } from "@dnd-kit/core";
import Piece from "./Piece";

function Square({ index, pieceLetter }) {
    const { isOver, setNodeRef } = useDroppable({
        id: "square-" + index,
    });

    const classes = [
        Math.floor(index / 8) % 2 == (index % 8) % 2
            ? "whiteSquare"
            : "blackSquare",
        isOver ? "hoverSquare" : undefined,
    ];

    return (
        <div ref={setNodeRef} className={classes.join(" ")}>
            {pieceLetter ? <Piece index={index} letter={pieceLetter} /> : <></>}
        </div>
    );
}

export default Square;
