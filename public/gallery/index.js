// Fetch stored pictures
const picturesContainer = document.getElementById("pictures-container");

fetch("/cgi_bin/fetch_stored_pictures.py", {
  method: "GET",
})
  .then(function (response) {
    return response.json();
  })
  .then(function (data) {
    // This is the JSON from our response
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

        const deleteButton = document.createElement("button");
        const deleteButtonText = document.createTextNode("🗑️");
        deleteButton.appendChild(deleteButtonText);
        deleteButton.setAttribute("id", "delete-button");
        deleteButton.setAttribute("type", "submit");
        deleteButton.setAttribute("data-value", imageUrl);

        link.appendChild(picture);
        li.appendChild(deleteButton);
        li.appendChild(link);
        picturesList.appendChild(li);

        deleteButton.addEventListener("click", deletePicture);
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

// Delete picture
const deletePicture = (e) => {
  const pictureToDelete = e.target.dataset.value;
  console.log(pictureToDelete);

  if (confirm("Do you want to delete this picture?")) {
    fetch(pictureToDelete, {
      method: "DELETE",
    })
      .then(function (response) {
        return response.json();
      })
      .then(function (data) {
        if (data.status !== 200) {
          window.alert("Error, try again!");
        }
        location.reload();
      })
      .catch(function (err) {
        // There was an error
        console.warn("Something went wrong.", err);
      });
  }
};
