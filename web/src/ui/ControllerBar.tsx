import {useState} from 'react';
import {Game} from '../helpers/game';
import {ImageLoader} from '../helpers/image_loader';
import {Icon} from './Icon';

export const ControllerBar = ({
  game,
  imageLoader,
}: {
  game: Game;
  imageLoader: ImageLoader;
}) => {
  const [loadImageInput] = useState(() => {
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = 'image/*';
    input.hidden = true;
    input.onchange = () => imageLoader.load(input.files?.[0]);
    document.body.append(input);
    return input;
  });

  return (
    <div
      w="16"
      display="flex"
      flex="col"
      justify="center"
      align="items-center"
      bg="black"
    >
      <ControllerIcon
        icon="show"
        onMouseDown={() => game.reset()}
        onMouseUp={() => game.undoReset()}
      />
      <ControllerIcon icon="shuffle" onClick={() => game.shuffle()} />
      <Divider />
      <ControllerIcon icon="add-row" onClick={() => ++game.gridHeight} />
      <ControllerIcon icon="delete-row" onClick={() => --game.gridHeight} />
      <Divider />
      <ControllerIcon icon="add-column" onClick={() => ++game.gridWidth} />
      <ControllerIcon icon="delete-column" onClick={() => --game.gridWidth} />
      <Divider />
      <ControllerIcon icon="image" onClick={() => loadImageInput.click()} />
    </div>
  );
};

const ControllerIcon = (props: {
  icon: string;
  onClick?: Function;
  [k: string]: unknown;
}) => {
  return (
    <Icon
      w="full"
      h="12"
      text="true-gray-700"
      transition="duration-150"
      hover="text-true-gray-400 h-14"
      active="text-true-gray-500"
      {...props}
    />
  );
};

const Divider = () => {
  return <div m="y-1" w="12" h="1px" bg="dark-50" />;
};
