# VCSEL Die Aperture Area Calculator (C++, OpenCV)

### VCSEL
>**Vertical-Cavity Surface-Emmiting Laser:** a semiconductor laser diode with an emission of laser beam perpendicular to the top surface.  
>![image](https://github.com/Josephood7/Automation/assets/144367420/a7dff5c1-a78f-489e-b67c-508431b1ee19)

### Oxide aperature diameter properties
>**Aperture diameter &#8593; :** Output power &#8593; Threshold current &#8593; Threshold current density &#8595;  
>It is crucial to measure the area, width ,and length of the aperture.  
>Modern laser exhibit a irregular shape of apertures.  
>![image](https://github.com/Josephood7/Automation/assets/144367420/875b1269-af09-4d27-ab30-5afea649a9ca)

### Steps
>I can not offer the most recent high quality .bmp images of the latest technology of VCSELs due to the NDA(Non-Disclosure Agreement). However, this approach will apply to all the images if you careful adjust the hyperparameters to your datasets.
> 1. Apply grayscale to preprocess the images from 3(RGB) to 1(gray) channels images.
> <img width="401" alt="ssssCapture" src="https://github.com/Josephood7/Automation/assets/144367420/d3013df6-f947-4c7a-931c-171b9446cf2a">

> 2. Use template matching tecnique to locate the aperture and crop to fit the detected region
> <img width="401" alt="Capture" src="https://github.com/Josephood7/Automation/assets/144367420/22c5eeb9-f6f8-4a29-8306-85a812e1767c"> 
> <img width="61" alt="1Capture" src="https://github.com/Josephood7/Automation/assets/144367420/d785362a-dc07-41b0-b1b1-969d1edec94d">  

> 3. Enhance and apply mask to stratify different areas by their own hue  
> <img width="126" alt="12Capture" src="https://github.com/Josephood7/Automation/assets/144367420/207e5473-8741-4b65-8767-8791d5c847a9">  

> 4. Detect boundaries  
> <img width="126" alt="3Capture" src="https://github.com/Josephood7/Automation/assets/144367420/9c78c19f-fe0c-455d-9b17-f7cb83074aa4">  

> 5. Draw countors and calculate the area, width, and length of the maximum countor  
> <img width="126" alt="5Capture" src="https://github.com/Josephood7/Automation/assets/144367420/be540ffa-2f6a-4f06-b257-5f830d862cdb">  
> <img width="111" alt="7Capture" src="https://github.com/Josephood7/Automation/assets/144367420/3040c816-43cf-42fb-aee8-4fe675916aaa">

> 6. Apply your own "pixel-to-um" ratio
> <img width="258" alt="77777Capture" src="https://github.com/Josephood7/Automation/assets/144367420/464d16ac-395d-48fd-81a8-a78b74e9a873">







