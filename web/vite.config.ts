/* eslint-disable node/no-unpublished-import */
import {defineConfig} from 'vite';
import pluginString from 'vite-plugin-string';
import pluginPurgeIcons from 'vite-plugin-purge-icons';
import pluginWindicss from 'vite-plugin-windicss';

export default defineConfig({
  plugins: [
    pluginString(),
    pluginPurgeIcons(),
    pluginWindicss({
      config: {attributify: true},
    }),
  ],
  esbuild: {
    jsxInject: 'import React from "react";',
  },
});
