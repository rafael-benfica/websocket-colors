const path = require("path");

const { defineConfig } = require("@vue/cli-service");
module.exports = defineConfig({
  transpileDependencies: ["vuetify"],
  configureWebpack: {
    resolve: {
      alias: {
        "@components": path.resolve(__dirname, "src/components"),
        "@services": path.resolve(__dirname, "src/services"),
      },
    },
  },
});
