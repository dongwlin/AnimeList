<script setup>
import { config } from "@/views/setting/edit/config.js";
import {open} from "@/api/open.js";
import {ElNotification} from "element-plus";
import { toType, toDay } from "@/utils/toValue.js";
import AnimeDescription from "@/views/index/components/AnimeDescription.vue";

defineProps({
  status: {
    type: Boolean,
    default: true,
  },
  imgSrc: {
    type: String,
  },
  name: {
    type: String,
    default: 'Example',
  },
  type: {
    type: Number,
    default: -1,
  },
  url: {
    type: String,
    default: '',
  },
  dir: {
    type: String,
    default: '',
  },
  day: {
    type: Number,
    default: -1,
  },
  description: {
    type: String,
    default: '',
  }
});

const openUrl = (url) => {
  window.open(url, '_blank');
}

const openDir = async (folderName) => {
  await open(folderName)
      .then(response => {
        if (response.code === 200) {
          ElNotification({
            title: 'Success',
            message: 'Open folder success.',
            type: 'success',
            position: 'bottom-right'
          });
        } else if (response.code === 500) {
          ElNotification({
            title: 'Error',
            message: 'Open folder fail.',
            type: 'error',
            position: 'bottom-right'
          });
        }
      })
      .catch(error => {
        console.log(error);
      });
}

const handleOpen = (type, url, dir) => {
  switch (type) {
    case config.type.none:
      break;
    case config.type.network:
      openUrl(url);
      break;
    case config.type.local:
      openDir(dir);
      break;
    default:
      break;
  }
}
</script>

<template>
  <div class="anime-container">
    <div class="img-container">
      <el-image
          class="img"
          :src="imgSrc"
          :fit="'cover'"
          :preview-src-list="[imgSrc]"
      >
        <template #error>
          <div class="image-slot">
            <span class="image-failed-text">Anime</span>
          </div>
        </template>
      </el-image>
      <AnimeDescription class="description" v-if="description" :content="description"></AnimeDescription>
    </div>
    <div class="anime-info">
      <el-card shadow="hover" @click="handleOpen(type, url, dir)">
        <el-text class="anime-name" truncated>{{ name }}</el-text>
      </el-card>
      <div class="tag-container">
        <el-tag :class="day !== config.day.none ? 'tag': 'tag-max'">{{ toType(type) }}</el-tag>
        <el-tag v-if="day !== config.day.none" class="tag">{{ toDay(day) }}</el-tag>
      </div>
    </div>
  </div>
</template>

<style scoped>
.anime-container {
  display: flex;
  width: 330px;
  align-items: center;
}

.anime-info {
  display: flex;
  width: 220px;
  height: 100%;
  justify-content: space-between;
  flex-direction: column;
}

.img-container {
  position: relative;
  width: 100px;
  height: 100px;
  margin-right: 10px;
  border-radius: 0.5rem;
}

.img {
  width: 100px;
  height: 100px;
  margin-right: 10px;
  border-radius: 0.5rem;
}

.description {
  position: absolute;
  bottom: 0;
  left: 0;
}

.el-card {
  width: 220px;
  margin-top: 3px;
}

.anime-name {
  width: 200px;
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

.tag-container {
  display: flex;
  justify-content: space-between;
}

.tag {
  width: 49%;
}

.tag-max {
  width: 100%;
}
</style>
