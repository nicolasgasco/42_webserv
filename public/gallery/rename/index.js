const renameForm = document.getElementById("rename-form");

const file = document.getElementById("file");
const newName = document.getElementById("name");

renameForm.addEventListener("submit", (e) => {
  e.preventDefault();
  console.log("mierda");

  console.log(renameForm.action);

  renameForm.action =
    "/cgi_bin/rename_picture.py?file=" + file.value + "&name=" + newName.value;

  console.log(renameForm.action);
  renameForm.submit();
});
