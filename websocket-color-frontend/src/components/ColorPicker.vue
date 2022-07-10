<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" md="3" class="d-flex justify-center">
        <v-color-picker
          v-if="colorLoaded"
          elevation="5"
          v-model="color"
          swatches-max-height="50vh"
          class="ma-2"
          show-swatches
          mode="hexa"
          @input="updateColorServer"
        />

        <v-skeleton-loader
          v-else
          style="width: 100%"
          class="ma-2"
          type="card-avatar"
        />
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import socket from "@services/websocket.js";
import  _ from 'lodash'
export default {
  name: "ColorPicker",

  data: () => ({
    color: null,
    colorLoaded: false
  }),
  methods: {
    updateColorServer:
      _.debounce((value)=>{
        console.log('setColor',value);
        socket.emit("setColor", value);
      }, 250),
  

    changeColor(data) {
      this.color = data;
      if (!this.colorLoaded) this.colorLoaded = true
      console.log("Color Changed", this.color);
    },
  },
  watch: {
    color(value) {
      this.$emit("colorChange", value);
    },
  },
  mounted () {
    console.log("cor", this.color);
    console.log("conectado", socket.connected);

    if (socket.connected) {
      socket.emit("getColor");
    } else {
      socket.connect();
    }
    socket.on("connect", () => {
      console.log("Conectado ->", socket.id);
      if (!this.color) {
        socket.emit("getColor");
      }
    });

    socket.on("disconnect", () => {
      console.log("Desconectado ->", socket.id);
    });

    socket.on("colorChange", (data) => {
      this.changeColor(data);
    });

    socket.on("getColor", (data) => {
      this.changeColor(data);
    });
  },
};
</script>
