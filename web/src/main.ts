import {GameScreen} from './game_screen';

const gameScreen = new GameScreen();

gameScreen.appendTo(document.body);

document.onkeydown = event => {
  gameScreen.handleKeyPress(event);
};
