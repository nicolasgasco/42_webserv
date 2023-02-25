// Fetch stored pictures
const picturesContainer = document.getElementById("pictures-container");

fetch("/cgi_bin/fetch_stored_pictures.py", {
  method: "GET",
})
  .then(function (response) {
    console.log(response.status, response.statusText, !response.ok);

    return response.json();
  })
  .then(function (data) {
    // This is the JSON from our response
    console.log(data);
    const picturesList = document.createElement("ul");
    picturesContainer.appendChild(picturesList);
    if (data.value.length) {
      for (let imageUrl of data.value) {
        const li = document.createElement("li");

        const link = document.createElement("a");
        link.href = imageUrl;

        const picture = document.createElement("img");
        picture.src = imageUrl;
        picture.alt = "";

        link.appendChild(picture);
        li.appendChild(link);
        picturesList.appendChild(li);
      }
    } else {
      picturesContainer.innerHTML =
        "<p>You currently have no pictures to show.</p>";
    }
  })
  .catch(function (err) {
    // There was an error
    console.warn("Something went wrong.", err);
  });
