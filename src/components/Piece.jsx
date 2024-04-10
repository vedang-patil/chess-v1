import { useDraggable } from "@dnd-kit/core";
import { CSS } from "@dnd-kit/utilities";

function Piece({ index, letter }) {
    const { attributes, listeners, setNodeRef, transform } = useDraggable({
        id: "piece-" + index,
    });

    let pieceClasses = [
        "piece",
        letter.toUpperCase() === letter ? "piece-white" : "piece-black",
    ];

    return (
        <div
            ref={setNodeRef}
            style={{
                transform: CSS.Translate.toString(transform),
                backgroundImage: `url(/pieces/${letter.toUpperCase()}.webp)`,
            }}
            {...listeners}
            {...attributes}
            className={pieceClasses.join(" ")}
        ></div>
    );
}

export default Piece;
