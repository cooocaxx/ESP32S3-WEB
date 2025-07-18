import { createRouter, createWebHistory } from 'vue-router'

const routes = [
  {
    path: '/',
    name: 'Home',
    component: () => import('@/views/home.vue'),
    meta: { title: 'Home' }
  },
 {
    path: '/upload',
    name: 'upload',
    component: () => import('@/views/system.vue') // 你需要有这个组件
  },
   {
    path: '/settings',
    name: 'settings',
    component: () => import('@/views/settings.vue') // 你需要有这个组件
  },
  {
  path: '/:pathMatch(.*)*',
  name: 'NotFound',
  component: () => import('@/views/NotFound.vue') // 404 页面组件
}
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router