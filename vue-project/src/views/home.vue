<template>
    <el-space wrap :size="10" :direction="direction">
        <!-- System Card -->
        <el-card>
            <template #header>
                <div class="card-header">
                    <span>SYSTEM</span>
                </div>
            </template>

            <el-descriptions column="2">
                <el-descriptions-item label="Device" label-class-name="custom-label">
                    <span>{{ data?.sys?.model }}</span>
                </el-descriptions-item>
                <el-descriptions-item label="Version">{{ data?.sys?.sdk }}</el-descriptions-item>
                <el-descriptions-item label="Freq"> {{ data?.sys?.arch.freq + 'Mhz' }}</el-descriptions-item>
                <el-descriptions-item label="Firmware version">{{ data?.sys?.fw }}</el-descriptions-item>
            </el-descriptions>

        </el-card>

        <!-- Memory Card -->
        <el-card>
            <template #header>
                <div class="card-header">
                    <span>MEMERY</span>
                </div>
            </template>

            <el-descriptions column="2">
                <el-descriptions-item label="Total. Availabl">{{ data?.mem?.total + 'KB' }}</el-descriptions-item>
                <el-descriptions-item label="Free">{{ data?.mem?.free + 'KB' }}</el-descriptions-item>
            </el-descriptions>
        </el-card>

        <el-card>
            <template #header>
                <div class="card-header">
                    <span>FileSystem</span>
                </div>
            </template>
            <el-descriptions column="1">
                <el-descriptions-item label="Total Availabl">{{ data?.mem?.total + 'KB' }}</el-descriptions-item>
                <el-descriptions-item label="User">{{ data?.fs?.free + 'KB' }}</el-descriptions-item>
                <el-descriptions-item label="Free">{{ data?.fs?.free + 'KB' }}</el-descriptions-item>
            </el-descriptions>
        </el-card>
        <!-- AP Card -->
        <el-card>
            <template #header>
                <div class="card-header">
                    <span>Wireless(AP)</span>
                </div>
            </template>
            <el-descriptions column="1">
                <el-descriptions-item label="SSID">
                    <el-input v-model="data.ap.ssid" style="width: 240px" type placeholder=""
                        :disabled="isApDisabled" />
                </el-descriptions-item>

                <el-descriptions-item label="PWD">
                    <el-input v-model="data.ap.pwd" style="width: 240px" type="password" placeholder=""
                        :disabled="isApDisabled" show-password />
                </el-descriptions-item>

                    <el-descriptions-item label="number of clients">
                        <span>{{ data.ap.num }}</span>

                      
                </el-descriptions-item>
            </el-descriptions>
            <template #footer>
                <div style="text-align: right; ">
                    <el-button type="danger" @click="change_input_disable('ap-card')">修改</el-button>
                    <el-popconfirm title="Are you sure to submit this? " @confirm="update_input('ap-card')"
                        @cancel="change_input_enable('ap-card')">
                        <template #reference>
                            <el-button :disabled="isApDisabled">submit</el-button>
                        </template>
                    </el-popconfirm>
                </div>
            </template>
        </el-card>
        <!-- STA Card -->
        <el-card>
            <template #header>
                <div class="card-header">
                    <span>Wireless(STA)</span>
                </div>
            </template>
            <el-descriptions column="1" class="sta-card">
                <el-descriptions-item label="SSID">
                    <el-input v-model="data.sta.ssid" style="width: 240px" placeholder="" :disabled="isStaDisabled" />
                </el-descriptions-item>

                <el-descriptions-item label="pwd ">
                    <el-input v-model="data.sta.pwd" style="width: 240px" type="password" placeholder=""
                        :disabled="isStaDisabled" show-password />
                </el-descriptions-item>

                <el-descriptions-item label="Status" type="success">
                    <el-text :type="data.sta.status ? 'success' : 'danger'"
                        style="font-size: 15px; font-weight: bold; ">
                        {{ data?.sta?.status ? "connected" : "unconnected" }}
                    </el-text>
                </el-descriptions-item>
            </el-descriptions>

            <template #footer>
                <div style="text-align: right;">
                    <el-button type="danger" @click="change_input_disable('sta-card')">修改</el-button>
                    <el-popconfirm title="Are you sure to submit this? " @confirm="update_input('sta-card')"
                        @cancel="change_input_enable('sta-card')">
                        <template #reference>
                            <el-button :disabled="isStaDisabled">submit</el-button>
                        </template>
                    </el-popconfirm>
                    <!-- <el-button type="primary" @click="" :disabled="isStaDisabled">提交</el-button> -->
                </div>
            </template>
        </el-card>
    </el-space>
</template>

<script lang="ts" setup>
import { reactive, onMounted, ref } from 'vue'
import { ElMessage } from 'element-plus'

import axios from 'axios'
const isStaDisabled = ref(true) // 独立的响应式变量控制 sta-card 输入框状态
const isApDisabled = ref(true) // 独立的响应式变量控制 sta-card 输入框状态
const direction = 'horizontal'

const change_input_disable = (className: string) => {
    if (className === 'sta-card') {
        isStaDisabled.value = false // 切换禁用状态
    } else {
        isApDisabled.value = false // 切换禁用状态
    }
}

const change_input_enable = (className: string) => {
    if (className === 'sta-card') {
        isStaDisabled.value = true // 切换禁用状态
    } else {
        isApDisabled.value = true // 切换禁用状态
    }
}

const update_input = (className: string) => {

    if (className === 'sta-card') {

        isStaDisabled.value = true // 切换禁用状态
        axios
            .post('/api/v1/config', {
                'wifi-sta': {
                    ssid: data.sta.ssid,
                    password: data.sta.pwd
                }
            })
            .then(response => {
                console.log('Response data:', response.data);
            })
            .catch(error => {
                if (error.response) {

                } else if (error.request) {
                    // 请求已发出，但没有收到响应
                    console.error('Error request:', error.request);
                } else {
                    // 其他错误
                    ElMessage.error('submit failed, please check the network or backend service.');
                }

            });

    } else {
        isApDisabled.value = true // 切换禁用状态
        // axios
        //     .post('/api/v1/wifi/ap', {
        //         ssid: data.ap.ssid,
        //         pwd: data.ap.pwd
        //     })
        //     .then(response => {
        //         console.log(response.data)
        //     })
        //     .catch(error => {
        //         console.log(error)
        //     })
    }
}

const data = reactive({
    sys: {
        model: "esp32-s3",
        fw: null,
        sdk: "v4.4.7-dirty",
        arch: {
            mfr: null,
            model: null,
            revision: null,
            cpu: null,
            freq: null
        }
    },
    mem: {
        total: 0,
        free: 0
    },
    fs: {
        total: 0,
        used: 0,
        free: 0
    },
    ap: {
        ssid: "dssdvs",
        pwd: null,
        num: null
    },
    sta: {
        ssid: "null",
        pwd: "asfafsaf",
        status: null
    }
})

onMounted(() => {
    axios
        .get('/api/v1/status')
        .then(response => {
            Object.assign(data, response.data)
        })
        .catch(error => {
            console.log(error)
        })
})
</script>

<style scoped>
.el-card {
    border-radius: 20px;
    width: 30vw;
    min-width: 380px;
    max-width: 440px;
    min-height: 240px;
}

.el-card:hover {
    transform: translateY(-5px);
    box-shadow: 0 4px 15px 0 rgba(0, 0, 0, 0.2);
}

.el-card {
    animation: fadeIn 0.5s ease-in-out;
}

@keyframes fadeIn {
    0% {
        opacity: 0;
        transform: translateY(20px);
    }

    100% {
        opacity: 1;
        transform: translateY(0);
    }
}

:deep(.el-card__header) {
    height: 36px;
    padding: 10px;
    /* padding: calc(var(--el-card-padding) ) var(--el-card-padding); */

}

.card-header {
    height: 100%;
    font-size: 15px;
    text-align: center;
    /* align-items: center; */
    font-weight: bold;
    padding: 0;
    font-family: 'Fira Mono', 'Consolas', 'Menlo', 'Courier New', Courier, monospace;
}

:deep(.el-descriptions__label) {
    font-size: 15px;
    font-weight: bold;
    font-family: 'Segoe UI', 'PingFang SC', 'Hiragino Sans GB', 'Microsoft YaHei', Arial, sans-serif;
}

:deep(.el-descriptions__content) {
    font-size: 15px;
    font-family: 'Fira Mono', 'Consolas', 'Menlo', 'Courier New', Courier, monospace;
    color: #2d8cf0;
    /* font-weight: bold; */
}
</style>
