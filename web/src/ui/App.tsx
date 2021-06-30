import {defineComponent, reactive, watchEffect} from 'vue';
import {Game} from '../helpers/game';
import {GridView} from './grid_view';

export const App = defineComponent(() => {
  const game = reactive(new Game());
  const canvas = document.createElement('canvas');
  const gridView = new GridView(game as Game, canvas);

  game.changeSelectedTile('Right');
  game.changeSelectedTile('Right');
  game.changeSelectedTile('Right');
  // game.changeSelectedTile('Right');
  console.log(game.moveSelectedTile());
  game.changeSelectedTile('Left');

  canvas.style.width = 'calc(100% - 0rem)';
  canvas.style.height = '100%';
  document.body.append(canvas);

  setTimeout(() => gridView.draw(), 100);
  // watchEffect(() => gridView.draw());

  return () => <div class="w-0 bg-blue-gray-600"></div>;
});
