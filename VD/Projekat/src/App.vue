<template>
	<Header />
	
	<main class="px-1">
		<router-view />
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
		Header,
	}
}
</script>

<style scoped>

</style>
