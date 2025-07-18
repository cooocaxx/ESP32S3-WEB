import { createApp } from 'vue'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import App from './App.vue'
import router from './router.js' // 新增

const app = createApp(App)

app.use(ElementPlus)
app.use(router) // 新增
app.mount('#app')