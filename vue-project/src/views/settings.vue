<template>
  <el-space wrap fill>
    <el-card>
      <template #header>
        <div class="card-header">
          <span>WIFI</span>
        </div>
      </template>
      <div class="el-switch-container">
        <span>STA MODE ()</span>
        <el-switch v-model="data.wifi_sta" class="el-sw1" />
      </div>
      <div class="el-switch-container">
        <span>AP MODE</span>
        <el-switch v-model="data.wifi_ap" class="el-sw2" />
      </div>

    </el-card>

    <el-card>
      <template #header>
        <div class="card-header">
          <span>Card name</span>
        </div>
      </template>
      <p v-for="o in 4" :key="o" class="text item">{{ 'List item ' + o }}</p>
      <template #footer>Footer content</template>
    </el-card>

  </el-space>
</template>

<script setup>
import { ref } from 'vue'
import { onMounted, reactive } from 'vue'
import axios from 'axios'
const data = reactive({
  wifi_ap: false,
  wifi_sta: false
})
// const sta_value = ref(true)
// const ap_value = ref(true)
onMounted(() => {
  axios
    .get('/api/v1/get-wifiMode')
    .then(response => {
      Object.assign(data, response.data)
      data.wifi_sta =response.data.wifi_sta === "true" ? true : false;
      data.wifi_ap = response.data.wifi_ap === "true" ? true : false;
    })
    .catch(error => {
      console.log(error)
    })
})
</script>

<style scoped>
:deep(.el-card__header) {
  padding: 10px;
}

.el-card {
  border-radius: 20px;
  width: 30vw;
  /* min-width: 380px;
    max-width: 440px; */
  min-height: 50px;
}

.el-switch-container {
  display: flex;
  align-items: center;
  justify-content: space-between;
  /* padding-left: 10px; */
}
</style>