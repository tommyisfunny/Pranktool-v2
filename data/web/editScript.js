window.onload = ()=>{
    const textArea = document.getElementById("editArea");
    textArea.addEventListener("input", () => {
        textArea.parentNode.dataset.replicatedValue = textArea.value;
    });
    textArea.parentNode.dataset.replicatedValue = textArea.value;


    const saveButton = document.getElementById("save");
    saveButton.addEventListener("click", () => {
        var url = new URL(window.location.href);
        var fileName = url.searchParams.get("payload");

        var textField = document.getElementById("editArea");

        var blob = new Blob([textField.value], {type: 'text/plain'});
        var file = new File([blob], fileName, {type: "text/plain"});

        var data = new FormData();
        data.append('file', file);
        
        fetch("/uploadPayload", {
            method: "POST",
            body: data
        }).then((response)=>{
                console.log(response);
                location.href = "/";
            }
          ).catch(
            error => console.log(error)
          );
    });
};
