/* eslint-disable node/no-unpublished-import */
import {defineConfig} from 'vite';
import pluginString from 'vite-plugin-string';

export default defineConfig({
  plugins: [pluginString()],
});
