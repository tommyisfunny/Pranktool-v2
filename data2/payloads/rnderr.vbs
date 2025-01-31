Dim max,min,rand
max=10 * 60 * 1000
min=3 * 60 * 1000
Randomize

do
    WScript.Sleep (randint(min, max))
    rand = randint(1, 3)
    Select Case rand
        Case 1
            MsgBox "Memory error at address 0x"+Hex(randint(&h100000, &hFFFFFF)), vbCritical OR vbOKOnly, "Ein Fehler ist aufgetreten!"
        Case 2
            MsgBox "Task succesfully failed", vbInformation OR vbOKOnly, "Finished!"
        Case 3
            MsgBox "Are you sure you want to share your camera to "+CStr(randint(0, 255))+"."+CStr(randint(0, 255))+"."+CStr(randint(0, 255))+"."+CStr(randint(0, 255))+"?", vbQuestion OR vbOKOnly
    End Select
loop

Function randint(min, max)
    randint = Int((max-min+1)*Rnd+min)
End Function