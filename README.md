# 프로젝트 설명
음악 정보 관리 시스템으로 DB에 저장되어있는 데이터를 확인 및 관리가 가능하다.

## Requiremnts
- MySQL
- MFC

## 주요 코드
```
mysql_init(&mysql);
	mysql_real_connect(&mysql, "localhost", "scott", "tiger", "musicmanager", 0, NULL, 0);
	CString querystr = "select M_Id, M_Name, S_Name, C_Name, M_Year,M_Genre	from music "
			"left join singer on music.S_Id = singer.S_Id "
			"left join composer on music.C_Id = composer.C_Id order by M_Name";
	mysql_query(&mysql, querystr);
```
메인 화면에서 music table의 정보를 읽어오는데, left join을 활용하여 singer ID와 composer ID는 singer Name과 composer Name으로 출력하여 사용자들이 정보를 알기 쉽도록 한다.

```
CString querystr = "select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id "
		"left join composer on music.C_Id = composer.C_Id";	// search edit에 아무것도 입력하지 않았을 경우

	if (m_strSearch.GetLength() != 0)	// 검색창에 문자를 입력한 경우
	{
		if (m_ctrlCombo.GetCurSel() == 0) {			// ID로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id " 
				"left join composer on music.C_Id = composer.C_Id where M_ID like '%%%s%%'", m_strSearch);
		}
		else if (m_ctrlCombo.GetCurSel() == 1) {	// Name으로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id "
				"left join composer on music.C_Id = composer.C_Id where M_Name like '%%%s%%'", m_strSearch);
		}
		else if (m_ctrlCombo.GetCurSel() == 2) {	// Singer로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id "
				"left join composer on music.C_Id = composer.C_Id where S_Name like '%%%s%%'", m_strSearch);
		}
		else if (m_ctrlCombo.GetCurSel() == 3) {	// Composer로 Search
			querystr.Format("select M_Id,M_Name, S_Name, C_Name, M_Year,M_Genre	from music	left join singer on music.S_Id = singer.S_Id "
				"left join composer on music.C_Id = composer.C_Id where C_Name like '%%%s%%'", m_strSearch);
		}
	}
```
메인 화면에서 검색기능을 구현할 때 검색창에 입력된 문자의 길이를 통해 검색 단어가 입력되었는지 확인한다. 만약 검색할 단어가 입력되었으면, 커서의 위치로 검색할 키워드를 찾는다.

```

```

## 구현 화면
![전체](https://user-images.githubusercontent.com/29061016/101756144-0dff7a00-3b19-11eb-838f-19c267926784.png)

음원 정보를 보여주며, 상단의 drop box에서 ID, Name, Singer, Composer 중 하나를 선택하고 검색하고자 하는 키워드를 입력하면 해당 키워드를 포함하는 결과를 보여준다.

![music](https://user-images.githubusercontent.com/29061016/101756193-1ce62c80-3b19-11eb-9fc8-563a2fca6dd9.png)

음원 테이블에 저장되어있는 정보를 출력하며 수정, 삭제, 삽입 기능을 수행할 수 있다.

![singer](https://user-images.githubusercontent.com/29061016/101756217-253e6780-3b19-11eb-9a2d-5882b00af6e7.png)

가수 테이블에 저장되어있는 정보를 출력하며 수정, 삭제, 삽입 기능을 수행할 수 있다.

![composer](https://user-images.githubusercontent.com/29061016/101756235-2a031b80-3b19-11eb-9bf0-a64c2f432376.png)

작곡가 테이블에 저장되어있는 정보를 출력하며 수정, 삭제, 삽입 기능을 수행할 수 있다.
