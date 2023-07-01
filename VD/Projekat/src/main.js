import { createApp } from "vue"
import { createI18n } from "vue-i18n"
import App from "./App.vue"
import "bootstrap";
import "bootstrap/dist/css/bootstrap.css";
import i18nMessages from "./i18n/messages";
import routes from "./routes";
import { createRouter, createWebHashHistory } from "vue-router";

const i18n = createI18n({
	locale: "rs",
	fallbackLocale: "en",
	messages: i18nMessages
});

const router = createRouter({
	history: createWebHashHistory(),
	routes
});

createApp(App)
	.use(i18n)
	.use(router)
	.mount("#app");