import {useEffect, useReducer, useRef, useState} from 'react';
import {Game} from '../helpers/game';
import {ImageLoader} from '../helpers/image_loader';
import {GameView} from './game_view';
import defaulImageSource from '../images/win_meme.png';

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

    document.onkeydown = ({key}) => {
      switch (key) {
        case 'ArrowDown':
          game.changeCurrentCell('Bottom');
          break;
        case 'ArrowUp':
          game.changeCurrentCell('Top');
          break;
        case 'ArrowLeft':
          game.changeCurrentCell('Left');
          break;
        case 'ArrowRight':
          game.changeCurrentCell('Right');
          break;
        case 'm':
          game.moveTileAtCurrentCell();
          break;
      }
      return;
    };
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
      <div className="w-40 bg-blue-gray-600">
        <input
          type="file"
          accept="image/*"
          style={{color: 'transparent', width: '100%'}}
          onChange={event => imageLoader.load(event.target.files?.[0])}
        />
      </div>
    </>
  );
};
