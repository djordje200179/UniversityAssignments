<template>
	<h1 class="display-4 text-white text-center mt-2 mb-4">
		{{ $t("welcomeText") }}
	</h1>

	<div class="container">
		<div class="row">
			<div class="col-lg-9 py-2 py-lg-0">
				<h1 class="display-6 mt-4 mb-1 text-white text-center">
					{{ $t("newestArtworks") }}
				</h1>

				<div id="artworksCarousel" class="carousel slide mx-auto" data-bs-ride="carousel">
					<div class="carousel-inner">
						<router-link v-for="(artwork, index) in artworks" :key="index" class="carousel-item"
							:class="{ active: index == 0 }" :to="{ name: 'artwork', params: { name: artwork.title } }">
							<img :src="artwork.thumbnail" class="d-block h-100 mx-auto" :alt="artwork.title">
						</router-link>
					</div>
					<button class="carousel-control-prev" type="button" data-bs-target="#artworksCarousel" data-bs-slide="prev">
						<span class="carousel-control-prev-icon" aria-hidden="true"></span>
					</button>
					<button class="carousel-control-next" type="button" data-bs-target="#artworksCarousel" data-bs-slide="next">
						<span class="carousel-control-next-icon" aria-hidden="true"></span>
					</button>
				</div>
			</div>

			<div class="col-lg-3 py-2 py-lg-0">
				<Banner />
			</div>
		</div>
	</div>
</template>

<script>
import artworks from "../artworks.json";
import artists from "../artists.json";
import Artwork from "../components/Artwork.vue";
import Banner from "../components/Banner.vue";

const newestOffers = [
	{
		title: "The Starry Night",
		bidder: "Vincent van Tartalja",
		amount: 1000000,
	},
	{
		title: "Mona Lisa",
		bidder: "Don Milicev",
		amount: 2000000,
	},
	{
		title: "The Scream",
		bidder: "Edvard Misic",
		amount: 3000000,
	}
]

export default {
	name: "IndexView",
	components: {
		Artwork,
		Banner
	},
	computed: {
		newestOffers() {
			return newestOffers;
		},
		artworks() {
			return artworks.slice(0, 3);
		},
		artists() {
			return artists.slice(0, 3);
		}
	}
}
</script>

<style scoped>
.artwork-link {
	text-decoration: none;
	color: inherit;
}

.carousel-item img {
	max-height: 400px;
}
</style>