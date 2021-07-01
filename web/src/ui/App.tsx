import {useEffect, useReducer, useRef, useState} from 'react';
import {Game} from '../helpers/game';
import {ImageLoader} from '../helpers/image_loader';
import defaulImageSource from '../images/win_meme.png';
import {Direction} from '../types/enum';
import {ControllerBar} from './ControllerBar';
import {GameView} from './game_view';

export const App = () => {
  const [, refresh] = useReducer(x => ++x, 0);

  const [game] = useState(() => new Game());
  const [gameView] = useState(() => new GameView());
  const gameViewRef = useRef<HTMLDivElement>(null);
  const [imageLoader] = useState(() => new ImageLoader(defaulImageSource));

  useEffect(() => {
    gameView.appendTo(gameViewRef.current!);
    game.onStateChange = refresh;
    gameView.onStateChange = refresh;
    imageLoader.onStateChange = () => {
      gameView.image = imageLoader.image;
      refresh();
    };

    document.onkeydown = ({key}) => onKeyDown(game, key);
  }, []);

  gameView.draw(
    game.gridSize,
    game.tilesByCell,
    game.blankCell,
    game.currentCell,
    imageLoader.imageSize
  );

  return (
    <>
      <div className="flex-grow" ref={gameViewRef}></div>
      <ControllerBar game={game} imageLoader={imageLoader} />
    </>
  );
};

const onKeyDown = (game: Game, key: string) => {
  switch (key) {
    case 'ArrowDown':
      game.changeCurrentCell(Direction.Bottom);
      break;
    case 'ArrowUp':
      game.changeCurrentCell(Direction.Top);
      break;
    case 'ArrowLeft':
      game.changeCurrentCell(Direction.Left);
      break;
    case 'ArrowRight':
      game.changeCurrentCell(Direction.Right);
      break;
    case 'm':
      game.moveTileAtCurrentCell();
      break;
  }
};
