/* eslint-disable node/no-unpublished-import */
import {defineConfig} from 'vite';
import pluginString from 'vite-plugin-string';
import pluginWindicss from 'vite-plugin-windicss';

export default defineConfig({
  plugins: [pluginString(), pluginWindicss()],
  esbuild: {
    jsxInject: 'import React from "react";',
  },
});
