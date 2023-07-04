<template>
	<div class="row row-cols-1 row-cols-sm-2 row-cols-md-3 row-cols-lg-4 mx-1 mt-1">
		<router-link v-for="artwork in artworks" :key="artwork.title" 
					 class="artwork-link mb-2 px-1 px-2" 
					 :to=" { name: 'artwork', params: { name: artwork.title} }">
			<Artwork :artwork="artwork" />
		</router-link>
	</div>
</template>

<script>
import artworks from "../artworks.json";
import Artwork from "../components/Artwork.vue";

export default {
	name: "SearchView",
	components: {
		Artwork
	},
	computed: {
		artworks() {
			return artworks.filter(artwork => artwork.title.toLowerCase().includes(this.$route.params.query.toLowerCase()));
		},
	}
}
</script>

<style scoped>
.artwork-link {
	text-decoration: none;
	color: inherit;
}
</style>