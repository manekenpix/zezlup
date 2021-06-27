import {Game} from './components/game';

const game = new Game();

game.appendTo(document.body);

document.onkeydown = event => {
  game.handleKeyPress(event);
};
