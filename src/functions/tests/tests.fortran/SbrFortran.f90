subroutine multrealvalue(value,mult,result)

implicit none
real*8 value
real*8 mult
real*8 result

result=value*mult

return

end


! ==============================================================================


subroutine multintvalue(value,mult,result)

implicit none
integer value
integer mult
integer result

result=value*mult

return
end


! ==============================================================================


subroutine catstrings(str1,str2,resultstr)

implicit none
character*(*) str1
character*(*) str2
character*(*) resultstr

resultstr=str1//' '//str2

return
end


! ==============================================================================


subroutine multrealmatrix(matrix,dim1,dim2,mult,result)

implicit none
integer dim1
integer dim2
real*8 matrix(dim2,dim1)
integer mult
real*8 result(dim2,dim1)
integer i, j, k

do j=1,dim1
  do i=1,dim2
    result(i,j) = matrix(i,j) * mult
  end do
end do


return
end
