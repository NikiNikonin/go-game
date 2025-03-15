import { motion } from "framer-motion";

const SelectionColumn = ({ title, options, selectedIndex, onSelect }: {
  title: string;
  options: string[];
  selectedIndex: number;
  onSelect: (index: number) => void;
}) => {
  return (
    <div className="flex flex-col items-center gap-2">
      <h2 className="text-2xl mb-4">{title}</h2>
      {options.map((option, index) => (
        <div
          key={index}
          onClick={() => onSelect(index)}
          className="relative cursor-pointer p-2 rounded-full w-40 text-center"
        >
          {selectedIndex === index && (
            <motion.div
              layoutId={`highlight-${title}`}
              className="absolute inset-0 bg-blue-500 rounded-full"
              transition={{ type: "spring", stiffness: 300, damping: 20, duration: 0.8 }}
            />
          )}
          <span className="relative z-10 text-white">{option}</span>
        </div>
      ))}
    </div>
  );
};

export default SelectionColumn;
