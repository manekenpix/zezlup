/* eslint-disable node/no-unpublished-import */
import {defineConfig} from 'vite';
import pluginVueJsx from '@vitejs/plugin-vue-jsx';
import pluginString from 'vite-plugin-string';
import pluginWindicss from 'vite-plugin-windicss';

export default defineConfig({
  plugins: [pluginVueJsx(), pluginString(), pluginWindicss()],
});
