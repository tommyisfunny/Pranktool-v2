var payload;
var payloadFiles;

var saveButton;
var runButton;
var backButton;

var payloadFilesContainer;
var textArea;
var fileUploadInput;

var heading;

var currentFile;


window.onload = async ()=>{
    saveButton = document.getElementById("save");
    runButton = document.getElementById("run");
    backButton = document.getElementById("back");

    payloadFilesContainer = document.getElementById("payloadFiles");
    textArea = document.getElementById("editArea");
    fileUploadInput = document.getElementById("payloadUploadSelector");
    heading = document.getElementById("heading");


    let response = await fetch("/getPayloads");
    var availablePayloads = await response.json();

    var url = new URL(window.location.href);
    payload = url.searchParams.get("payload");
    payloadFiles = availablePayloads[payload];

    heading.innerHTML = "Edit " + payload;

    console.log(payload);
    console.log(availablePayloads);
    console.log(payloadFiles);

    setupTextArea();
    createFileTable();
    setupSaveButton();
    setupFileUpload(); 
    setupRunButton();
    setupBackButton();
};

function setupTextArea(){
    textArea.value = "Press the edit button to edit a file";
    textArea.addEventListener("input", () => {
        textArea.parentNode.dataset.replicatedValue = textArea.value;
    });
    textArea.parentNode.dataset.replicatedValue = textArea.value;
}

function setupSaveButton(){
    saveButton.addEventListener("click", () => {
        var fileName = currentFile;
        console.log(fileName);

        var textField = document.getElementById("editArea");

        var blob = new Blob([textField.value], {type: 'text/plain'});

        var data = new FormData();
        data.append('file', blob, fileName);
        
        fetch("/uploadFile?path=" + payload + "/", {
            method: "POST",
            body: data
        }).then((response)=>{
                console.log(response);
            }
          ).catch(
            error => console.log(error)
          );
    });
}

function createFileTable(){
    var table = document.createElement("table");
    payloadFiles.forEach((file)=>{
        var tr = document.createElement("tr");
        var th = document.createElement("th");
        th.innerHTML = file;
        tr.appendChild(th);

        th = document.createElement("th");
        th.appendChild(createEditButton(file));
        tr.appendChild(th);

        th = document.createElement("th");
        th.appendChild(createDeleteButton(file));
        tr.appendChild(th);

        table.appendChild(tr);
    });

    payloadFilesContainer.appendChild(table);
}

function createEditButton(file){
    var button = document.createElement("button");
    button.innerHTML = "Edit";
    button.onclick = function(){
        loadFile(file);
    }
    return button;
}

function createDeleteButton(file){
    var button = document.createElement("button");
    button.innerHTML = "Delete";
    button.onclick = function(){
        deleteFile(file);
    }
    return button;
}

async function loadFile(file){
    currentFile = file;
    var response = await fetch("/getFile?file=" + payload + "/" + file);
    text = await response.text();
    console.log(text);
    textArea.value = text;
    textArea.parentNode.dataset.replicatedValue = text;
    heading.innerHTML = "Edit " + payload + "/" + file;
}

function deleteFile(file){
    fetch("/deleteFile?file=" + payload + "/" + file).then(()=>{
        location.reload();
    });
}

function setupFileUpload(){
    fileUploadInput.addEventListener("change", ()=>{
        var file = fileUploadInput.files[0];
        var data = new FormData();
        data.append('file', file);
        
        fetch("/uploadFile?path=" + payload + "/", {
            method: "POST",
            body: data
        }).then((response)=>{
                console.log(response) // if the response is a JSON object
                location.reload()
            }
          ).catch(
            error => console.log(error) // Handle the error response object
          );
        
          
    });
}

function setupRunButton(){
    runButton.onclick = ()=>{
        fetch("/runPayload?payload=" + payload);
    }
}

function setupBackButton(){
    backButton.onclick = ()=>{
        location.href = "/";
    }
}