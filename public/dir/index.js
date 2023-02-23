const dirContentContainer = document.querySelector("#dir-content");

const dirContentContainerInnerText = dirContentContainer.innerText;
dirContentContainer.innerHTML = `<ul>
${dirContentContainerInnerText
  .split(",")
  .map((file) => {
    if (file.length) {
      return `<li>${file}</li>`;
    }
  })
  .join("")}
</ul>`;
