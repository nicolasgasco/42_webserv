// Send DELETE request when clicking on delete button
const deleteButtons = document.querySelectorAll(".delete-button");
deleteButtons.forEach((item) => {
  item.addEventListener("click", (e) => {
    const pictureToDelete = e.target.dataset.value;

    if (confirm("Do you want to delete this picture?")) {
      fetch(pictureToDelete, {
        method: "DELETE",
      })
        .then(function (response) {
          if (!response.ok) {
            throw new Error(`${response.status} - ${response.statusText}`);
          }
          return response.json();
        })
        .then(function () {
          location.reload();
        })
        .catch(function (err) {
          // There was an error
          window.alert(err);
        });
    }
  });
});
