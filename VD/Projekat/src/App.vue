<template>
	<Header />

	<main class="p-1 flex-grow-1">
		<router-view :key="$route.fullPath" />
	</main>

	<Footer />
</template>

<script>
import Header from "@/components/Header.vue";
import Footer from "@/components/Footer.vue";

export default {
	name: 'App',
	components: {
		Footer,
		Header
	},
	watch: {
        $route: {
            immediate: true,
            handler(to, from) {
				const viewName = to.name + "View";
                document.title = `${this.$t("title")} - ${this.$t(viewName)}`;
            }
        },
    }
}
</script>

<style scoped>
main {
	background-color: #272947;
}
</style>