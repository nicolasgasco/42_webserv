// Send DELETE request when clicking on delete button
const deleteButtons = document.querySelectorAll(".delete-button");
deleteButtons.forEach((item) => {
  console.log(item);
  item.addEventListener("click", (e) => {
    const pictureToDelete = e.target.dataset.value;

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
  });
});
