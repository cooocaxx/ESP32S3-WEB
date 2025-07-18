<template>
  <div class="firmware-uploader">
    <h3>ESP 设备固件更新</h3>
    <el-upload
      ref="uploadRef"
      class="upload-demo"
      name="update"
      action="/api/v1/update"
      :limit="1"
      :auto-upload="false"
      :on-exceed="handleExceed"
      :on-success="handleSuccess"
      :on-error="handleError"
      accept=".bin"
    >
      <!-- 插槽 #trigger 用于定义触发文件选择的元素 -->
      <template #trigger>
        <el-button type="primary">选择固件 (.bin)</el-button>
      </template>

      <!-- 独立的上传按钮 -->
      <el-button class="ml-3" type="success" @click="submitUpload">
        上传到设备
      </el-button>

      <!-- 插槽 #tip 用于显示提示信息 -->
      <template #tip>
        <div class="el-upload__tip">
          请选择一个 .bin 文件进行更新。新的文件会覆盖旧的选择。
        </div>
      </template>
    </el-upload>
  </div>
</template>

<script lang="ts" setup>
import { ref } from 'vue'
import { ElMessage } from 'element-plus'
import type { UploadInstance, UploadProps, UploadRawFile, UploadUserFile } from 'element-plus'

// 1. 创建一个 ref 来获取 el-upload 组件的实例
const uploadRef = ref<UploadInstance>()

// 2. "上传到设备" 按钮的点击事件处理函数
const submitUpload = () => {
  // 调用 el-upload 组件的 submit 方法，这会开始上传文件
  // 如果没有选择文件，uploadRef.value 将是 undefined，所以用可选链 ?.
  uploadRef.value?.submit()
}

// 3. 上传成功时的回调函数
// response: 服务器返回的响应体 (你的 C++ 代码中的 "OK" 或 "FAIL")
// uploadFile: 当前上传的文件信息对象
const handleSuccess: UploadProps['onSuccess'] = (response, uploadFile) => {
  // 清空已上传文件列表，为下次上传做准备
  uploadRef.value?.clearFiles()
  
  if (response === 'OK') {
    ElMessage({
      message: '固件更新成功！设备正在重启，请稍后...',
      type: 'success',
      duration: 5000, // 消息显示5秒
    })
  } else {
    ElMessage.error('固件更新失败！服务器返回 "FAIL"。请检查设备串口日志获取详细错误。')
  }
}

// 4. 上传失败时的回调函数 (网络问题或服务器返回非 200 状态码)
const handleError: UploadProps['onError'] = (error: Error) => {
  ElMessage.error(`上传失败: ${error.message}`)
}

// 5. 当选择的文件超出限制（我们设置了 limit=1）时的处理函数
const handleExceed: UploadProps['onExceed'] = (files) => {
  // 先清空已有的文件列表
  uploadRef.value!.clearFiles()
  // 把新选择的文件加进去
  const file = files[0] as UploadRawFile
  uploadRef.value!.handleStart(file)
  ElMessage.warning('已将上传文件替换为最新选择的文件。')
}
</script>

<style scoped>
.firmware-uploader {
  padding: 20px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  width: 500px;
  margin: 50px auto;
}
.ml-3 {
  margin-left: 12px;
}
.el-upload__tip {
  margin-top: 10px;
  color: #909399;
}
</style>