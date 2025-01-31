var settingsContainer;
var saveSettingsButton;
var settingsSavedDialog;

var settingsData;
var settingsInfo;
var availablePayloads;


export async function handleSettings(){
    settingsContainer = document.getElementById("settings");
    saveSettingsButton = document.getElementById("saveSettings");
    settingsSavedDialog = document.getElementById("settingsSavedDialog");

    let response = await fetch("/getSettings");
    settingsData = await response.json();

    response = await fetch("/getSettingsInfo");
    settingsInfo = await response.json();

    response = await fetch("/getPayloads");
    availablePayloads = await response.json();

    console.log("Hello")
    console.log(settingsData);
    console.log(settingsInfo);
    console.log(availablePayloads);

    createSettingsTable();
    setupSaveSettings();
}

function createSettingsTable(){
    var fields = settingsInfo.fields;

    availablePayloads.push("OFF");

    var table = document.createElement("table");

    fields.forEach((field)=>{
        var tr = document.createElement("tr");
        var th = document.createElement("th");
        th.innerHTML = field.lable;
        tr.appendChild(th);

        th = document.createElement("th");
        var input;

        if(field.type != "select"){
            input = document.createElement("input");

            input.id = field.name;
            if(field.type == "string") input.type = "text";
            if(field.type == "number") input.type = "number";
            if(field.type == "boolean") input.type = "checkbox";
            
            if(field.type == "boolean") input.checked = settingsData[field.name];
            else                        input.value = settingsData[field.name];
        } else {
            input = document.createElement("select");
            input.id = field.name;

            if(field.selectFrom == "payloads") addSelectOptions(input, availablePayloads, settingsData[field.name]);
            
        }

        th.appendChild(input);
        tr.appendChild(th);

        table.appendChild(tr);
    });

    settingsContainer.appendChild(table);
}

function addSelectOptions(selectElement, options, selected=""){
    options.forEach((option)=>{
        var optionElement = document.createElement("option");
        optionElement.value = option;
        optionElement.innerText = option;
        if(option == selected){
            optionElement.selected = true;
        }
        selectElement.appendChild(optionElement);
    });
}

function setupSaveSettings(){
    saveSettingsButton.onclick = async ()=>{
        var newSettings = {};

        settingsInfo.fields.forEach((field)=>{
            var input = document.getElementById(field.name);
            if(field.type == "boolean") newSettings[field.name] = input.checked;
            else                        newSettings[field.name] = input.value;
        });

        console.log(newSettings);

        var response = await fetch("/saveSettings", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(newSettings)
        });

        if(response.ok){
            settingsSavedDialog.showModal();
            window.setTimeout(()=>{
                settingsSavedDialog.close();
            }, 2000);
        } else {
            alert("Error saving settings");
        }
    }
}