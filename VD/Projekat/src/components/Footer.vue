<template>
	<footer class="text-center mt-auto">
		<img v-if="$i18n.locale == 'rs'" src="flag-uk.png" height="30" class="float-end d-block me-2 mt-2"
			 @click="$root.$i18n.locale = 'en'">
		<img v-if="$i18n.locale == 'en'" src="flag-serbia.png" height="30" class="float-end d-block me-2 mt-2"
			 @click="$root.$i18n.locale = 'rs'">

		<p class="m-0">{{ $t("copyrightText") }}</p>
		<p class="m-0">{{ $t("universityText") }}</p>
	</footer>
</template>

<script>
export default {
	name: "Footer"
}
</script>

<style scoped>
footer {
	background-color: #8688b0;
}
</style>