<template>
  <div class="common-layout">


    <el-container>
      <el-header height="40px" style="background-color: #eeeeef; border-radius: 10px;">
        <div class="header-logo">
          <img src="./assets/logo.svg" @click="goHome" width="20px" height="20px" alt=""><span> </span>
        </div>
      </el-header>
    </el-container>


    <el-container>

      <el-aside class="el-side-bar">
        <!-- <el-radio-group v-model="isCollapse" style="margin-bottom: 20px">
          <el-radio-button :value="false">expand</el-radio-button>
          <el-radio-button :value="true">collapse</el-radio-button>
        </el-radio-group> -->
        <!-- <el-scrollbar> -->
        <el-menu class="el-menu-vertical-demo" :collapse="isCollapse" @open="handleOpen"
          @close="handleClose" router>

          <el-menu-item index="/">
            <el-icon>
              <HomeFilled />
            </el-icon>
            <span> HOME</span>

          </el-menu-item>

          <el-menu-item index="/settings">
            <el-icon>
              <Tools />
            </el-icon>
            <span>SETTING</span>
          </el-menu-item>

          <el-menu-item index="/upload">
            <el-icon>
              <UploadFilled />
            </el-icon>
            <span>UPDATE</span>
          </el-menu-item>

        </el-menu>
        <!-- </el-scrollbar> -->
      </el-aside>



      <el-main>
        <router-view />
      </el-main>
    </el-container>

  </div>
</template>

<script lang="ts" setup>
import { ref } from 'vue'
import { Menu as Tools, HomeFilled, UploadFilled, InfoFilled } from '@element-plus/icons-vue'
import { useRouter } from 'vue-router'
const router = useRouter()
const isCollapse = ref(false)
const handleOpen = (key: string, keyPath: string[]) => {
  console.log(key, keyPath)
}
const handleClose = (key: string, keyPath: string[]) => {
  console.log(key, keyPath)
}
function goHome() {
  router.push('/')
}
</script>

<style scoped>
.el-menu-vertical-demo:not(.el-menu--collapse) {
  width: 200px;
  /* min-height: 400px; */
}


.header-logo {
  display: flex;
  align-items: center;
  justify-content: center;
  /* 水平居中 */
  height: 100%;
  width: 100%;
  border-radius: 50px;
}

.el-menu-item {
  font-size: 15px;
  font-weight: bold;
  font-family: 'Segoe UI', 'PingFang SC', 'Hiragino Sans GB', 'Microsoft YaHei', Arial, sans-serif;
}

.layout-container-demo .el-header {
  position: relative;
  /* background-color: var(--el-color-primary-light-7); */
  /* color: var(--el-text-color-primary); */
}

.el-aside {
  height: 100%;
  width: 150px;
  position: relative;
  top: 10px;
  /* margin-top: 10px; */
  border-radius: 20px;
  border-radius: 10px;

  /* color: var(--el-text-color-primary); */
  /* background: var(--el-color-primary-light-8); */
}

.el-menu-item {
  background-color: #eeeeef;
}

.layout-container-demo .el-menu {
  border-right: none;
}

.layout-container-demo .el-main {
  padding: 0;
}

.layout-container-demo .toolbar {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  height: 100%;
  right: 20px;
}
</style>
