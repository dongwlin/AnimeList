<script setup>
import { ref } from "vue";
import { config } from "@/views/setting/edit/config.js";
import { toDay, toType } from "@/utils/toValue.js";
import { create } from "@/api/create.js";
import { ElMessage } from "element-plus";
import useAnimeList from "@/store/modules/animeList.js";

const formRef = ref();
const form = ref({
  name: '',
  status: true,
  type: -1,
  dir: '',
  url: '',
  img: '',
  day: -1,
  description: '',
});

defineProps({
  visible: {
    type: Boolean,
    default: false
  }
});

const rules = ref(config.createFormRules);

const emit = defineEmits(['close']);

const replaceBackslash = (obj) => {
  obj.dir = obj.dir.replaceAll('\\', '/');
}

const handleImgSuccess = (response, uploadFile) => {
  form.value.img = response.data.img;
}

const beforeImgUpload = (rawFile) => {
  if (rawFile.type !== 'image/jpeg' && rawFile.type !== 'image/png') {
    ElMessage.error('Image must be JPG/PNG format!');
    return false;
  }
  else if (rawFile.size / 1024 / 1024 > 5) {
    ElMessage.error('Image size can not exceed 5MB!');
    return false;
  }
  return true;
}

const handleClose = (formEl) => {
  if (!formEl) return;
  formEl.resetFields();
  emit('close');
}

const requestCreate = async () => {
  let formData = new FormData();
  formData.append('name', form.value.name);
  formData.append('status', form.value.status);
  formData.append('type', form.value.type);
  formData.append('dir', form.value.dir);
  formData.append('url', form.value.url);
  formData.append('img', form.value.img);
  formData.append('day', form.value.day);
  formData.append('description', form.value.description);
  await create(formData)
      .then(() => {
        useAnimeList().handleList();
        ElMessage({
          message: 'success',
          type: 'success'
        });
      })
      .catch(error => {
        console.log(error);
        ElMessage({
          message: 'failed',
          type: 'error'
        });
      });
}

const handleCreate = async (formEl) => {
  if (!formEl) return;
  await formEl.validate((valid, fields) => {
    if (valid) {
      requestCreate();
      handleClose(formEl);
    }
  });
}
</script>

<template>
  <el-dialog :model-value="visible" title="Create" @close="handleClose(formRef)">
    <el-form :model="form" :rules="rules"  ref="formRef" hide-required-asterisk>
      <el-form-item label="Name" label-width="20%" prop="name">
        <el-col :span="20">
          <el-input v-model="form.name"/>
        </el-col>
      </el-form-item>
      <el-form-item label="Status" label-width="20%" prop="status">
       <el-switch v-model="form.status"></el-switch>
      </el-form-item>
      <el-form-item label="Type" label-width="20%" prop="type">
        <el-select v-model="form.type" placeholder="Please select a type">
          <el-option
              v-for="(item, index) in config.type"
              :key="index"
              :label="toType(item)"
              :value="item"
          ></el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="Dir" label-width="20%" prop="dir">
        <el-col :span="20">
          <el-input v-model="form.dir" @change="replaceBackslash(form)"></el-input>
        </el-col>
      </el-form-item>
      <el-form-item label="Url" label-width="20%" prop="url">
        <el-col :span="20">
          <el-input v-model="form.url"></el-input>
        </el-col>
      </el-form-item>
      <el-form-item label="Img" label-width="20%" prop="img">
        <el-col :span="20">
          <el-upload
              class="img-uploader"
              action="/uploadImage"
              name="image"
              :show-file-list="false"
              :on-success="handleImgSuccess"
              :before-upload="beforeImgUpload"
          >
            <el-image
                v-if="form.img"
                :src="form.img"
                :fit="'cover'"
                class="img"
            >
              <template #error>
                <div class="image-slot">
                  <span class="image-failed-text">Anime</span>
                </div>
              </template>
            </el-image>
            <el-icon v-else class="img-uploader-icon">
              <Plus></Plus>
            </el-icon>
          </el-upload>
        </el-col>
      </el-form-item>
      <el-form-item label="Day" label-width="20%" prop="day">
        <el-select v-model="form.day" placeholder="Please select a day">
          <el-option
              v-for="item in config.day"
              :label="toDay(item)"
              :value="item"
          ></el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="Description" label-width="20%" prop="description">
        <el-col :span="20">
          <el-input v-model="form.description"></el-input>
        </el-col>
      </el-form-item>
    </el-form>
    <template #footer>
      <span class="dialog-footer">
        <el-button @click="handleClose(formRef)">Cancel</el-button>
        <el-button type="primary" @click="handleCreate(formRef)">
          Confirm
        </el-button>
      </span>
    </template>
  </el-dialog>
</template>

<style scoped>
.img-uploader {
  border: 1px dashed var(--el-border-color);
  border-radius: 0.5rem;
  width: 100px;
  height: 100px;
  transition: var(--el-transition-duration-fast);
}

.img-uploader:hover {
  border-color: var(--el-color-primary);
}

.img {
  width: 100px;
  height: 100px;
  border-radius: 0.5rem;
}

.img-uploader-icon {
  font-size: 20px;
  width: 100px;
  height: 100px;
}

.image-slot {
  width: 100%;
  height: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
  background: #f5f7fa;
}

.image-failed-text {
  font-size: 20px;
  font-weight: bolder;
  color: #b1b3b8;
}
</style>
