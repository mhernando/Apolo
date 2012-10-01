#include "apoloMex.h" //de momento inutil ...habrá que borrarlo 
#include "../apoloMessage.h"


// Definimos la función de interfaz entre Matlab y C++
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	// Obtener los valores de entrada
	PixelEnt* imgEnt = (PixelEnt*)mxGetPr(prhs[0]);
	mwSize numDims = mxGetNumberOfDimensions(prhs[0]);
	const mwSize* dimImgEnt = mxGetDimensions(prhs[0]);

	// Crear y asignar los punteros de salida
	plhs[0] = mxCreateNumericMatrix(dimImgEnt[0],dimImgEnt[1],mxUINT8_CLASS,mxREAL);
	PixelSal* imgSal = (PixelSal*)mxGetPr(plhs[0]);

	// Llamada a la funcion
	binomial(imgEnt,numDims,dimImgEnt,imgSal);
}

void binomial(PixelEnt* imgEnt,mwSize numDims,const mwSize* dimImgEnt,PixelSal* imgSal) {

	// Calculamos el numero de píxeles (o voxels) que tiene nuestra imagen
	int i,nrow = dimImgEnt[0],ncol = dimImgEnt[1],numElem = ncol*nrow;
	
	// Pasamos a double
	double* imgEntDouble = new double[numElem];
	for(i=0;i<numElem;i++)
		imgEntDouble[i] = double(imgEnt[i]);

	// Creamos imagen de salida
	double* imgSalDouble = new double[numElem];

	// Recorremos imagen aplicando filtro
	// 4 esquinas
	imgSalDouble[0] = imgEntDouble[0]*.5625+imgEntDouble[1]*.1875+imgEntDouble[nrow]*.1875+imgEntDouble[nrow+1]*.0625;
	imgSalDouble[nrow-1] = imgEntDouble[nrow-1]*.5625+imgEntDouble[nrow-2]*.1875+imgEntDouble[2*nrow-1]*.1875+imgEntDouble[2*nrow-2]*.0625;
	imgSalDouble[nrow*(ncol-1)] = imgEntDouble[nrow*(ncol-1)]*.5625+imgEntDouble[nrow*(ncol-1)+1]*.1875+imgEntDouble[nrow*(ncol-2)]*.1875+
		imgEntDouble[nrow*(ncol-2)+1]*.0625;
	imgSalDouble[numElem-1] = imgEntDouble[numElem-1]*.5625+imgEntDouble[numElem-2]*.1875+imgEntDouble[numElem-nrow-1]*.1875+
		imgEntDouble[numElem-nrow-2]*.0625;
	// Columna izquierda
	for(i=1;i<nrow-1;i++) {
		imgSalDouble[i] = imgEntDouble[i]*.375+imgEntDouble[i-1]*.1875+imgEntDouble[i+1]*.1875+imgEntDouble[i+nrow]*.125+
			imgEntDouble[i+nrow-1]*.0625+imgEntDouble[i+nrow+1]*.0625;
	}
	// Columna derecha
	for(i=nrow*(ncol-1)+1;i<numElem-1;i++) {
		imgSalDouble[i] = imgEntDouble[i]*.375+imgEntDouble[i-1]*.1875+imgEntDouble[i+1]*.1875+imgEntDouble[i-nrow]*.125+
			imgEntDouble[i-nrow-1]*.0625+imgEntDouble[i-nrow+1]*.0625;
	}
	// Fila superior
	for(i=nrow;i<nrow*(ncol-1);i+=nrow) {
		imgSalDouble[i] = imgEntDouble[i]*.375+imgEntDouble[i-nrow]*.1875+imgEntDouble[i+nrow]*.1875+imgEntDouble[i+1]*.125+
			imgEntDouble[i-nrow+1]*.0625+imgEntDouble[i+nrow+1]*.0625;
	}
	// Fila inferior
	for(i=2*nrow-1;i<numElem-1;i+=nrow) {
		imgSalDouble[i] = imgEntDouble[i]*.375+imgEntDouble[i-nrow]*.1875+imgEntDouble[i+nrow]*.1875+imgEntDouble[i-1]*.125+
			imgEntDouble[i-nrow-1]*.0625+imgEntDouble[i+nrow-1]*.0625;
	}
	// Parte sin bordes de la imagen
	for(i=nrow+1;i<nrow*(ncol-1)-1;i++) {
		if(i%nrow!=0 && i%nrow!=nrow-1) {
			imgSalDouble[i] = imgEntDouble[i]*.25+imgEntDouble[i-1]*.125+imgEntDouble[i+1]*.125+imgEntDouble[i-nrow]*.125+
				imgEntDouble[i+nrow]*.125+imgEntDouble[i-1-nrow]*.0625+imgEntDouble[i-1+nrow]*.0625+imgEntDouble[i+1-nrow]*.0625+
				imgEntDouble[i+1+nrow]*.0625;
		}
	}
	
	// Devolvemos imagen como unsigned short despues de redondear al entero más cercano (redondeo empleado
	// solo valido para números positivos)
	for(i=0;i<numElem;i++)
		imgSal[i] = PixelSal(imgSalDouble[i]+.5);

	delete[] imgEntDouble;
	delete[] imgSalDouble;
}

