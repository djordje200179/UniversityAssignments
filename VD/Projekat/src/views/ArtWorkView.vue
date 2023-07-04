<template>
	<h1 class="text-center text-white pt-2">{{artwork.title}}</h1>

	<div class="container">
		<div class="row my-2 rounded p-4">
			<div class="col-lg-4">
				<img class="mx-auto d-block rounded" :src="artwork.thumbnail" :alt="artwork.name" id="art_image">
			</div>
			
			<div class="col-lg-8 mt-2 mt-lg-0">
				<h4 class="text-center">
					{{ $t("aboutAuthor") }} {{ artist.name }}
				</h4>

				<p>{{artist.biography[$i18n.locale]}}</p>
			
				<hr />

				<div class="d-flex bd-highlight">
					<div class="flex-grow-1">
						<small class="text-muted">
							{{ $t("estimatedPrice") }}: {{ artwork.price }}
						</small>
						<br/>
						<small class="text-muted">
							{{ $t("creationYear") }}: {{ artwork.year }}
						</small>
					</div>
				</div>		
			</div>
		</div>
	</div>

	<h1 class="display-6 mt-3 mb-1 text-white text-center">
		{{ $t("galleryOfImages") }}
	</h1>
	<div id="image-gallery" class="carousel slide mx-auto" data-bs-ride="true">
		<div class="carousel-inner">
			<div v-for="(image, index) in artwork.gallery" :key="image.id" 
				 class="carousel-item" :class="{active: index == 0}">
				<img :src="image.link" class="d-block w-100">
			</div>

			<div v-if="artwork.video" class="carousel-item">
				<iframe width="560" height="315" :src="artwork.video" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
			</div>
		</div>

		<button class="carousel-control-prev" type="button" data-bs-target="#image-gallery" data-bs-slide="prev">
			<span class="carousel-control-prev-icon"></span>
		</button>
		<button class="carousel-control-next" type="button" data-bs-target="#image-gallery" data-bs-slide="next">
			<span class="carousel-control-next-icon"></span>
		</button>
	</div>
	
	<h1 class="display-6 mt-3 mb-1 text-white text-center">
		{{ $t("receivedOffers") }}
	</h1>
	<table class="table table-dark mx-auto text-center shrinked-element">
		<thead>
			<tr>
				<th scope="col">{{ $t("user") }}</th>
				<th scope="col">{{ $t("amount") }}</th>
			</tr>
		</thead>
		<tbody>
			<tr v-for="bid in bids" :key="bid.id">
				<td>{{ bid.bidder }}</td>
				<td>{{ bid.amount }}</td>
			</tr>
		</tbody>
	</table>
	<form class="input-group shrinked-element mx-auto my-2" @submit="addBid">
		<input type="text" class="form-control" :placeholder="$t('giveYourOffer')"
			   name="offerAmount" required>
		<button class="btn btn-light">{{ $t("submitOffer") }}</button>
	</form>
	<div v-if="offerSubmitted" class="alert alert-success fade show shrinked-element mx-auto"
		 @click="offerSubmitted = false" role="alert">
		{{ $t("yourOfferWasSubmitted") }}
	</div>
	
	<h1 class="display-6 mt-3 mb-1 text-white text-center">
		{{ $t("comments") }}
	</h1>
	<table class="table table-dark mx-auto text-center shrinked-element">
		<thead>
			<tr>
				<th scope="col">{{ $t("user") }}</th>
				<th scope="col">{{ $t("comment") }}</th>
			</tr>
		</thead>
		<tbody>
			<tr v-for="comment in artwork.comments" :key="comment.id">
				<td>{{ comment.commenter }}</td>
				<td>{{ comment.comment }}</td>
			</tr>
		</tbody>
	</table>
	<form class="input-group shrinked-element mx-auto my-2" @submit="addComment">
		<input type="text" class="form-control" :placeholder="$t('enterYourComment')"
			   name="commentText" required>
		<button class="btn btn-light">{{ $t("sendComment") }}</button>
	</form>
	<div v-if="commentPosted" class="alert alert-success fade show shrinked-element mx-auto"
		 @click="commentPosted = false" role="alert">
		{{ $t("yourCommentWasPosted") }}
	</div>
</template>

<script>
import artworks from "../artworks.json";
import artists from "../artists.json";

export default {
	name: "ArtworkView",
	data() {
		return {
			offerSubmitted: false,
			commentPosted: false
		}
	},
	computed: {
		artwork() {
			return artworks.find(artwork => artwork.title == this.$route.params.name);
		},
		artist() {
			return artists.find(artist => artist.artworks.find(artwork => artwork == this.$route.params.name));
		}, 
		bids() {
			const artwork = artworks.find(artwork => artwork.title == this.$route.params.name);
			return artwork.bids.slice().sort((first, second) => first.amount <= second.amount ? 1 : -1);
		}
	},
	methods:{
		addBid(event) {
			event.preventDefault();

			const formData = new FormData(event.target);
			const offerAmount = formData.get("offerAmount");

			const newBid = {
				offer: offerAmount,
				artwork: this.artwork.title
			};

			const currentBids = JSON.parse(localStorage.getItem("bids") || "[]");
			currentBids.push(newBid);
			localStorage.setItem("bids", JSON.stringify(currentBids));

			this.offerSubmitted = true;
		},
		addComment(event) {
			event.preventDefault();

			const formData = new FormData(event.target);
			const commentText = formData.get("commentText");

			const newComment = {
				comment: commentText,
				artwork: this.artwork.title
			};

			const currentComments = JSON.parse(localStorage.getItem("comments") || "[]");
			currentComments.push(newComment);
			localStorage.setItem("comments", JSON.stringify(currentComments));

			this.commentPosted = true;
		}
	}
}
</script>

<style scoped>
.row {
	background-color: #8688b0;
}

.row img {
	max-width: 100%;
	object-fit: contain;
}

#image-gallery {
	max-height: 600px;
	max-width: 600px;
}

#image-gallery img{
	max-width: 100%;
	max-height: 600px;
}

.shrinked-element {
	min-width: 400px;
	max-width: 60%;
}

#art_image{
	max-width: 100%;
	max-height: 500px;
}
</style>