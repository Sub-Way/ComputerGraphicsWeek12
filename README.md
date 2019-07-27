# ComputerGraphicsWeek12

## 실습 12주차. Subdivision Surfaces

### 문제. Thingiverse에서 낮은 해상도(low resolution)의 3D 모델을 선택하여 Subdivision을 적용하기.
-	Step 1. Butterfly subdivision의 적용전과 후를 비교하시오. (30점)
#### 적용 전
![image](https://user-images.githubusercontent.com/22046757/61996457-86ee6500-b0cf-11e9-80d8-5d4c02819721.png) 
#### 적용후
![image](https://user-images.githubusercontent.com/22046757/61996461-881f9200-b0cf-11e9-9e6f-f16328cc5718.png)

surface.advanceOneSubdivisionStep(1); 메소드를 이용하여 적용하였다.
모델이 좀 더 곡선화되었다.
-	Step 2. Loop subdivision의 적용전과 후를 비교하시오.  (30점)
#### 적용 전
![image](https://user-images.githubusercontent.com/22046757/61996462-8a81ec00-b0cf-11e9-801d-35a4f463cd21.png) 
#### 적용 후
![image](https://user-images.githubusercontent.com/22046757/61996463-8bb31900-b0cf-11e9-88a2-b0a847a51acc.png)

surface.advanceOneSubdivisionStep(1); 메소드를 이용하여 적용하였다.  
모델이 더 자연스럽게 곡선화되었다.

-	Step 3. Butterfly, Loop, Linear subdivision을 혼합해서 smoothing 효과를 만들기. (40점)
#### 혼합 적용
![image](https://user-images.githubusercontent.com/22046757/61996466-91106380-b0cf-11e9-93c7-426039b7e08e.png) 
	surface.advanceOneSubdivisionStep(0);
	surface.advanceOneSubdivisionStep(1);
	surface.advanceOneSubdivisionStep(2);
메소드를 이용하여 적용하였다.  
이미지가 부드러워진 것을 확인할 수 있다.

-	Extra. Subdivision한 결과물을 3D 프린터로 출력해보기.
Subdivision을 적용한 3D 모델을 .stl 파일로 저장한 후, 신공학관 3147호에 가셔서 3D 프린터 출력 신청을 하시면 됩니다.
STL 파일 저장은 surface.writeSTL(“저장할 파일 이름”) 함수를 이용하면 됩니다.

