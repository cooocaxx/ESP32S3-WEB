import { fileURLToPath } from 'url'

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import viteCompression from 'vite-plugin-compression'
import pluginRewriteAll from 'vite-plugin-rewrite-all'; // <--- 1. 导入新插件

// https://vitejs.dev/config/
export default defineConfig({
  
  plugins: [
    vue(),
    pluginRewriteAll(),
    // viteCompression(
    // {
    //   threshold: 102400, // Compress files larger than 10KB
    //   deleteOriginFile: false, // Keep original files
    // })
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  },
  // server: {
  //   port: 5173,
  //   host: '0.0.0.0',
  //   proxy: {
  //     '/api': 'http://localhost:8081'
  //   }
  // },
  build: {
    outDir: '../data/www',
    reportCompressedSize : true, // enable report of compressed size
    emptyOutDir: true, // clear the output directory before building
  }
})
