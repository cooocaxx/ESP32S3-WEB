<script setup lang="ts">
import { useRouter } from 'vue-router'
const router = useRouter()
function goHome() {
  router.push('/')
}
</script>

<template>
  <el-empty>
    <el-button type="primary" @click="goHome">HOME</el-button>
  </el-empty>
</template>