import {createApp} from "vue"
import {createI18n} from "vue-i18n"
import App from "./App.vue"
import "bootstrap/dist/css/bootstrap.css";
import i18nMessages from "./i18n/messages";

const i18n = createI18n({
	locale: "rs",
	fallbackLocale: "en",
	messages: i18nMessages
});

const app = createApp(App)

app.use(i18n)

app.mount("#app")