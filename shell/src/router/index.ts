/**
 * Vue Router Configuration
 *
 * Defines application routes for the NUXP shell.
 */

import { createRouter, createWebHistory, type RouteRecordRaw } from 'vue-router';

const routes: RouteRecordRaw[] = [
  {
    path: '/design',
    name: 'design-demo',
    component: () => import('@/views/DesignDemo.vue'),
  },
  {
    path: '/tests',
    name: 'tests',
    component: () => import('@/views/TestsView.vue'),
  },
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
